#include "Enemy.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Animation/AnimMontage.h"
#include "Engine/Engine.h"
#include "Projectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h" // Hinzufügen für AIController
#include "NavigationSystem.h" // Für Navigation
#include <Components/WidgetComponent.h>

// Konstruktor
AEnemy::AEnemy()
{
    // Setze diese Klasse als Tick-fähig
    PrimaryActorTick.bCanEverTick = true;
    HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
    HealthBarWidget->SetupAttachment(RootComponent);

    HealthBarWidget->SetDrawSize(FVector2D(200.0f, 50.0f)); // Breite x Höhe
    HealthBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f)); // Position über dem Gegner
    HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

// BeginPlay: Wird beim Spielstart aufgerufen
void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    // Holen des AIControllers, um Bewegungsbefehle zu geben
    //AIController = Cast<AAIController>(GetController());

    // Suche den Spieler zu Beginn des Spiels
    FindPlayer();
}

// Tick: Wird jede Frame aufgerufen
void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!canAttack) {
        bool battack = CHeckLineOfSideToPLayer();
        FollowPlayer(DeltaTime);
        if (battack) {
            if (PlayerCharacter)
            {


                if (FVector::Dist(PlayerCharacter->GetActorLocation(), GetActorLocation()) < 200.0f && !canAttack && MeeleEnemy) {
                   // UE_LOG(LogTemp, Warning, TEXT("Abstand zum Spieler: %f. Bedingung erfüllt, Angriff wird ausgeführt."), FVector::Dist(PlayerCharacter->GetActorLocation(), GetActorLocation()));

                    DoAnimation(AttackMontage);
                    GetCharacterMovement()->MaxWalkSpeed = 0.0f;
                    canAttack = true;
                    FTimerHandle speedTimer;
                    GetWorld()->GetTimerManager().SetTimer(speedTimer, this, &AEnemy::ResetSpeed, AttackMontage->GetPlayLength(), false);
                    FTimerHandle attackTimer;
                    GetWorld()->GetTimerManager().SetTimer(attackTimer, this, &AEnemy::ResetCanAttack, attackSpeed, false);
                }

                if (FVector::Dist(PlayerCharacter->GetActorLocation(), GetActorLocation()) < 1000.0f && !canAttack && RangeEnemy) {
                //    UE_LOG(LogTemp, Warning, TEXT("Abstand zum Spieler: %f. Bedingung erfüllt, Angriff wird ausgeführt."), FVector::Dist(PlayerCharacter->GetActorLocation(), GetActorLocation()));

                    DoAnimation(AttackMontage);
                    GetCharacterMovement()->MaxWalkSpeed = 0.0f;
                    FTimerHandle speedTimer;
                    GetWorld()->GetTimerManager().SetTimer(speedTimer, this, &AEnemy::ResetSpeed, AttackMontage->GetPlayLength(), false);
                    RangeAttack();
                    canAttack = true;
                    FTimerHandle attackTimer;
                    GetWorld()->GetTimerManager().SetTimer(attackTimer, this, &AEnemy::ResetCanAttack, attackSpeed, false);
                }
            }
        }
    }

    // Wenn der Spieler gefunden wurde, folge ihm
}

// Methode, um den Spieler zu finden, basierend auf dem Tag "Player"
void AEnemy::FindPlayer()
{
    // Finde den Spieler im Spiel, der den Tag "Player" hat
    TArray<AActor*> FoundPlayers;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), FoundPlayers);

    // Falls ein Spieler gefunden wurde, speichere ihn in der PlayerCharacter-Variable
    if (FoundPlayers.Num() > 0)
    {
        PlayerCharacter = Cast<APlayerCharacter>(FoundPlayers[0]);
    }
}

// Methode, um dem Spieler zu folgen und einen Abstand von 50 Einheiten zu halten
void AEnemy::FollowPlayer(float DeltaTime)
{
    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController && PlayerCharacter)
    {
        FVector DirectionToPlayer = PlayerCharacter->GetActorLocation() - GetActorLocation();
        float Distance = DirectionToPlayer.Size();

        // Wenn der Gegner zu nah ist, bewege ihn zurück
        if (Distance < FollowDistance)
        {
            FVector TargetLocation = GetActorLocation() - DirectionToPlayer.GetSafeNormal() * FollowDistance;
            AIController->MoveToLocation(TargetLocation);
        }
        // Wenn der Gegner zu weit weg ist, bewege ihn näher
        else
        {
            AIController->MoveToActor(PlayerCharacter);
        }
    }
}

// Methode, um die Angriffsanimation abzuspielen
void AEnemy::Attack()
{
    if (AttackMontage)
    {
        // Führe die Angriffsanimation aus
        PlayAnimMontage(AttackMontage);
    }
}

void AEnemy::RangeAttack()
{
    if (ProjectileClass) // Überprüfe, ob eine Projektil-Klasse zugewiesen wurde
    {
        // Erhalte die Spawn-Position und Rotation basierend auf dem Charakter
        FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f; // Spawn vor dem Charakter
        FRotator SpawnRotation = GetActorRotation();

        // Spawne das Projektil
        AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);

        if (SpawnedProjectile)
        {
            // Weitere Initialisierungen, falls erforderlich (z.B. Geschwindigkeit einstellen)
            // SpawnedProjectile->MoveSpeed = 1200.0f; // Beispiel: Geschwindigkeit setzen
        }
    }
}

void AEnemy::GetDMG(float dmgGet)
{
    if (life > 0) {
        life -= dmgGet;
        //Destroy();
    }

    if (life <= 0.0f) {
        PlayerCharacter->GEtSkillPoint(1.0f);
        Destroy();
    }
}

void AEnemy::DoAnimation(UAnimMontage* attackMontage)
{
    // Überprüfen, ob der Angriffsmontage gültig ist
    if (!attackMontage)
    {
        UE_LOG(LogTemp, Warning, TEXT("Angriffs-Montage ist ungültig!"));
        return; // Falls keine gültige Montage übergeben wurde, abbrechen
    }

    // Holen der Animationsinstanz des Mesh
    UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
    if (animInstance)
    {
        // Montage abspielen
        animInstance->Montage_Play(attackMontage, 1.0f);
        UE_LOG(LogTemp, Warning, TEXT("Angriffsanimation wird abgespielt."));
    }
    else
    {
        // Fehlerbehandlung, wenn die Animationsinstanz ungültig ist
        UE_LOG(LogTemp, Warning, TEXT("AnimInstance ist ungültig!"));
    }
}

void AEnemy::ResetCanAttack()
{
    canAttack = false;
}

void AEnemy::ResetSpeed()
{
    PlayerCharacter->GetDMG(5.0f);
    GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

bool AEnemy::CHeckLineOfSideToPLayer()
{
    if (!PlayerCharacter) return false;

    FVector Start = GetActorLocation(); // Der Startpunkt des Tracers ist der Gegner
    FVector ForwardVector = GetActorForwardVector();
    FVector End = Start + (ForwardVector * 4000.0f); // Das Ziel ist die Position des Spielers

    // Führe den LineTrace aus
    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this); // Ignoriere den eigenen Gegner

    // Schieße einen LineTrace
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);
    //DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);

    if (bHit && HitResult.GetActor())
    {
    //    UE_LOG(LogTemp, Log, TEXT("Spieler in Sichtlinie gefunden!"));
       // UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
        // Wenn der Trace ein Actor trifft, prüfe, ob der Actor den Tag "Player" hat
        if (HitResult.GetActor()->ActorHasTag(FName("Player")))
        {
        //    UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
            return true;
        }
    }

    // Wenn der Trace etwas anderes trifft oder nicht trifft
    return false;
}
