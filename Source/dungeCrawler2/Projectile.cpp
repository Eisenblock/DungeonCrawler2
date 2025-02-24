#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Enemy.h"

// Konstruktor
AProjectile::AProjectile()
{
    // Setze diese Klasse als Tick-fähig
    PrimaryActorTick.bCanEverTick = true;

    // Erstelle die SphereComponent für die Kollision
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->InitSphereRadius(15.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
    RootComponent = CollisionComponent;

    // Setze die Overlap-Funktion
   CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
    CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AProjectile::OnOverlapEnd);

    CollisionComponentExplode = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent_Exlode"));
    CollisionComponentExplode->InitSphereRadius(15.0f);
    CollisionComponentExplode->SetupAttachment(RootComponent);
    CollisionComponentExplode->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    CollisionComponentExplode->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBeginExplode);
    // Erstelle das Static Mesh für das Projektil
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(RootComponent);

    // Erstelle die Niagara-Komponente
    NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
    NiagaraEffect->SetupAttachment(RootComponent);
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();
    TArray<AActor*> FoundPlayers;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), FoundPlayers);

    // Falls ein Spieler gefunden wurde, speichere ihn in der PlayerCharacter-Variable
    if (FoundPlayers.Num() > 0)
    {
        PlayerCharacter = Cast<APlayerCharacter>(FoundPlayers[0]);
    }
    // Stelle sicher, dass die Niagara-Komponente ein VFX-System zugewiesen bekommt
    if (NiagaraEffect)
    {
        NiagaraEffect->Activate();
    }

    FTimerHandle timerLifeTime;
    GetWorld()->GetTimerManager().SetTimer(timerLifeTime, this, &AProjectile::DEstroyObject,lifetime, true);

}

void AProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Bewege das Projektil kontinuierlich nach vorne
    FVector ForwardDirection = GetActorForwardVector();
    FVector NewLocation = GetActorLocation() + (ForwardDirection * MoveSpeed * DeltaTime);
    SetActorLocation(NewLocation);
}

// Overlap-Funktion: Wird aufgerufen, wenn das Projektil mit etwas kollidiert
void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag(FName("Enemy")) && Player && autoProjetile)
    {
        // Hier kannst du etwas tun, z.B. Schaden anwenden, VFX oder Sounds abspielen
        UE_LOG(LogTemp, Warning, TEXT("Overlap detected with %s"), *OtherActor->GetName());

        // Zerstöre das Projektil
       
        enemy = Cast<AEnemy>(OtherActor);
        enemy->GetDMG(PlayerCharacter->dmgAuto);
        Destroy();
        //Destroy(enemy);
    }

    if (OtherActor && OtherActor->ActorHasTag(FName("Enemy")) && Player && explodeProjetile)
    {
        // Hier kannst du etwas tun, z.B. Schaden anwenden, VFX oder Sounds abspielen
        UE_LOG(LogTemp, Warning, TEXT("Overlap detected with %s"), *OtherActor->GetName());

        // Zerstöre das Projektil
       // Destroy();
        enemy = Cast<AEnemy>(OtherActor);
        //enemy->GetDMG(100.0f);
        ActivateExplodde();
        //Destroy(enemy);
    }

    if (OtherActor && OtherActor->ActorHasTag(FName("Enemy")) && Player && overTimeProjetile)
    {
       // UE_LOG(LogTemp, Warning, TEXT("Overlap detected with %s"), *OtherActor->GetName());

        //Destroy();
        enemy = Cast<AEnemy>(OtherActor);

        if (enemy && !AffectedEnemies.Contains(enemy))
        {
            AffectedEnemies.Add(enemy);

            // Für jeden neuen Gegner einen individuellen Timer setzen
            FTimerHandle NewTimerHandle;
            GetWorld()->GetTimerManager().SetTimer(NewTimerHandle, this, &AProjectile::DoDmgOverTime, 1.0f, true);

            // Den Timer für diesen Gegner speichern
            EnemyTimers.Add(enemy, NewTimerHandle);
        }
    }

    if (OtherActor && OtherActor->ActorHasTag(FName("Player")) && Enemy )
    {
        // Hier kannst du etwas tun, z.B. Schaden anwenden, VFX oder Sounds abspielen
        UE_LOG(LogTemp, Warning, TEXT("Overlap detected with %s"), *OtherActor->GetName());

        // Zerstöre das Projektil
       
        APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
        player->GetDMG(5.0f);
        Destroy();
        //Destroy(enemy);
    }
}

void AProjectile::OnOverlapBeginExplode(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag(FName("Enemy")) && Player && explodeProjetile)
    {
        // Hier kannst du etwas tun, z.B. Schaden anwenden, VFX oder Sounds abspielen
        UE_LOG(LogTemp, Warning, TEXT("Overlap detected with %s"), *OtherActor->GetName());

        // Zerstöre das Projektil
      //  Destroy();
        enemy = Cast<AEnemy>(OtherActor);
        enemy->GetDMG(PlayerCharacter->dmgExplode);
        //Destroy(enemy);
    }
}

void AProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
      UE_LOG(LogTemp, Warning, TEXT("Overlap reset detected with %s"), *OtherActor->GetName());
    AEnemy* enemyLost = Cast<AEnemy>(OtherActor);

    if (enemyLost)
    {
        // Entferne den Gegner aus der Liste der betroffenen Gegner
        AffectedEnemies.Remove(enemyLost);

        // Lösche den Timer für diesen speziellen Gegner
        if (EnemyTimers.Contains(enemyLost))
        {
            GetWorld()->GetTimerManager().ClearTimer(EnemyTimers[enemyLost]);
            EnemyTimers.Remove(enemyLost); // Entferne den Timer aus der Map
        }
    }

    // Wenn keine Gegner mehr vorhanden sind, lösche den globalen Timer
    if (AffectedEnemies.Num() == 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(dmgoverTimer);
    }
}

void AProjectile::ActivateExplodde()
{
    if (CleaveEffect)
    {
        // Position des Actors (kann auch eine andere Position sein, z.B. Offset)
        FVector SpawnLocation = GetActorLocation();

        // Spawne das Niagara-System an der Position des Actors
        UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),      // World context
            CleaveEffect,    // Das Niagara-System, das gespawnt werden soll
            SpawnLocation,   // Position zum Spawnen
            GetActorRotation() // Die Rotation des Actors (oder eine andere Rotation)
        );

        // Optional: Zerstöre das Partikelsystem nach 2 Sekunden
        if (SpawnedEffect)
        {
            FTimerHandle ParticleDestroyTimer;
            GetWorldTimerManager().SetTimer(ParticleDestroyTimer, [SpawnedEffect]()
                {
                    SpawnedEffect->DestroyComponent(); // Zerstöre das Partikelsystem nach 2 Sekunden
                }, explodeLifeTime, false);
        }
    }
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CollisionComponentExplode->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    MoveSpeed = 0;
    FTimerHandle timerExplode;
    GetWorld()->GetTimerManager().SetTimer(timerExplode, this, & AProjectile::DEstroyObject, explodeLifeTime, true);
}

void AProjectile::DoDmgOverTime()
{
    for (AEnemy* enemyTarget : AffectedEnemies)
    {
        if (enemyTarget)
        {
            enemyTarget->GetDMG(10.0f);
        }
    }
}

void AProjectile::DEstroyObject()
{
    Destroy();
}

