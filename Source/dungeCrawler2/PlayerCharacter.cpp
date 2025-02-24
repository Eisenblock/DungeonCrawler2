#include "PlayerCharacter.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"
#include "Enemy.h"
#include "NiagaraFunctionLibrary.h"
#include "Animation/AnimInstance.h"
#include <Kismet/GameplayStatics.h>
#include "Item.h"

APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    // Create a camera boom...
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
    CameraBoom->TargetArmLength = 800.f;
    CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

    // Create a camera...
    TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    TopDownCameraComponent->bUsePawnControlRotation = false;

    autoTimerC = cd_autoAttack;
    explodeTimerC = cd_ExplodeAttack;
    thunderTimerC = cd_ThunderAttack;
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->bShowMouseCursor = true;
    }
    AnimInstance = GetMesh()->GetAnimInstance();

    if (m_cPlayerHUD != nullptr)
    {
        //add the HUd to the viewport
       UUserWidget* HUD = CreateWidget<UUserWidget>(Cast<APlayerController>(GetController()), m_cPlayerHUD);
       HUD->AddToViewport(9999);

    }
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_DoAllReg, this, &APlayerCharacter::DoAllReg, 1.0f, true);
  //  GetChestItem();
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

        RotateToMouse(); 
    
     // Maussteuerung aktivieren
    CheckAllAbilityTImer(DeltaTime);

    // Bewegung analysieren
    // Berechnung des Dot-Products mit den entsprechenden Vektoren
    FVector ForwardVector = GetActorForwardVector();  // Vorwärts-Richtung
    FVector RightVector = GetActorRightVector();      // Rechts-Richtung
    FVector MovementDirection = GetVelocity().GetSafeNormal();  // Bewegungsrichtung des Spielers

    // Wenn die Velocity Null ist (der Charakter steht still), setze alle Bewegungsflags auf false
    if (MovementDirection.IsNearlyZero())
    {
        moveLeft = false;
        moceRight = false;
        moveForward = false;
        moveBack = false;
    }

    if (life <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Spiel beendet! Player Life ist 0."));

        // Option 1: Spiel pausieren
        UGameplayStatics::SetGamePaused(GetWorld(), true);

        // Option 2: Spiel verlassen (zurück zum Hauptmenü oder Desktop)
        // APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        // UGameplayStatics::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
    }
    // Dot-Product berechnen
    float DotForward = FVector::DotProduct(MovementDirection, ForwardVector);  // Vorwärts/Rückwärts
    float DotRight = FVector::DotProduct(MovementDirection, RightVector);      // Rechts/Links

    // Vorwärts laufen
    if (DotForward > 0.5f)
    {
        // AnimInstance->Montage_Play(RunForwardAnim);
        moveLeft = false;
        moceRight = false;
        moveBack = false;
        moveForward = true;
    }

    // Rückwärts laufen
    if (DotForward < -0.5f)
    {
        // AnimInstance->Montage_Play(RunBackwardAnim);
        moveLeft = false;
        moceRight = false;
        moveForward = false;
        moveBack = true;
    }

    // Rechts laufen
    if (DotRight > 0.5f)
    {
        // AnimInstance->Montage_Play(RunRightAnim);
        moveForward = false;
        moveBack = false;
        moveLeft = false;
        moceRight = true;
    }

    // Links laufen
    if (DotRight < -0.5f)
    {
        // AnimInstance->Montage_Play(RunLeftAnim);
        moveForward = false;
        moveBack = false;
        moceRight = false;
        moveLeft = true;
    }

    GetChestItem();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
    PlayerInputComponent->BindAction("ShotAuto", IE_Pressed, this, &APlayerCharacter::ShotBallLightning);
    PlayerInputComponent->BindAction("I_Ability", IE_Pressed, this, &APlayerCharacter::StartExplode);
    PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &APlayerCharacter::StartBlink);
    PlayerInputComponent->BindAction("Thunder", IE_Pressed, this, &APlayerCharacter::StartThunder);
}


void APlayerCharacter::MoveForward(float Value)
{
    if (Value == 0.0f) return; // Verhindert unnötige Berechnungen

    MovementInput.X = Value;

    // Kamerarotation abrufen
    FRotator CameraRotation = Controller->GetControlRotation();
    FRotator YawRotation(0, CameraRotation.Yaw, 0); // Nur die horizontale Rotation verwenden

    // Forward-Vektor basierend auf der Kamera
    FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

    AddMovementInput(ForwardDirection, Value * MoveSpeed);
}

void APlayerCharacter::MoveRight(float Value)
{
    if (Value == 0.0f) return;

    MovementInput.Y = Value;

    // Kamerarotation abrufen
    FRotator CameraRotation = Controller->GetControlRotation();
    FRotator YawRotation(0, CameraRotation.Yaw, 0);

    // Right-Vektor basierend auf der Kamera
    FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    AddMovementInput(RightDirection, Value * MoveSpeed);
}

void APlayerCharacter::RotateToMouse()
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC || bIsRolling) return;

    FVector WorldLocation, WorldDirection;
    if (PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
    {
        // LineTrace, um die Mausposition auf dem Boden zu finden
        FHitResult HitResult;
        FVector TraceStart = WorldLocation;
        FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0f); // Weit nach vorne projizieren

        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this); // Spieler ignorieren

        if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
        {
            FVector TargetLocation = HitResult.Location;
            FVector DirectionToMouse = (TargetLocation - GetActorLocation()).GetSafeNormal();

            // Nur die Yaw-Achse drehen (keine Neigung nach oben/unten)
            FRotator TargetRotation = UKismetMathLibrary::MakeRotFromX(DirectionToMouse);
            TargetRotation.Pitch = 0.0f;
            TargetRotation.Roll = 0.0f;

            // Den gesamten Charakter drehen (nicht nur das Mesh!)
            SetActorRotation(TargetRotation);
        }
    }
}

void APlayerCharacter::ShotBallLightning()
{
    if (ProjectileClass && !autoAttackB) // Überprüfe, ob eine Projektil-Klasse zugewiesen wurde
    {
        //DoAnimation(autoMon);
        // Erhalte die Spawn-Position und Rotation basierend auf dem Charakter
        FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f; // Spawn vor dem Charakter
        FRotator SpawnRotation = GetActorRotation();
        autoAttackB = true;
        if (mana_current < mana_max) {
            mana_current += getManaAuto;
        }
       

        // Spawne das Projektil
        AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);

        if (SpawnedProjectile)
        {
            // Weitere Initialisierungen, falls erforderlich (z.B. Geschwindigkeit einstellen)
            // SpawnedProjectile->MoveSpeed = 1200.0f; // Beispiel: Geschwindigkeit setzen
        }
    }
}

void APlayerCharacter::DoThunder()
{
    if (!thunderB && mana_current >= manaCostThunder) {
        thunderB = true;
        mana_current -= manaCostThunder;
       // DoAnimation(thunderMon);
        FVector ActorLocation = GetActorLocation();

        // Liste aller Actors mit dem Tag "Enemy"
        TArray<AActor*> TaggedEnemies;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Enemy"), TaggedEnemies);

        for (AActor* Enemy : TaggedEnemies)
        {
            if (Enemy)
            {
                // Berechne die Distanz zwischen dem Ziel und dem Gegner
                float Distance = FVector::Dist(ActorLocation, Enemy->GetActorLocation());

                // Prüfe, ob der Gegner innerhalb von 20 Einheiten ist
                if (Distance <= 2000.0f)
                {
                    // Füge dem Gegner Schaden zu (via GetDmg-Methode)
                    AEnemy* EnemyCharacter = Cast<AEnemy>(Enemy);
                    if (EnemyCharacter)
                    {
                        if (CleaveEffect)
                        {
                            // Position des Actors (kann auch eine andere Position sein, z.B. Offset)
                            FVector SpawnLocation = EnemyCharacter->GetActorLocation();

                            // Spawne das Niagara-System an der Position des Actors
                            UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                                GetWorld(),      // World context
                                CleaveEffect,    // Das Niagara-System, das gespawnt werden soll
                                SpawnLocation,   // Position zum Spawnen
                                EnemyCharacter->GetActorRotation() // Die Rotation des Actors (oder eine andere Rotation)
                            );

                            // Optional: Zerstöre das Partikelsystem nach 2 Sekunden
                            if (SpawnedEffect)
                            {
                                FTimerHandle ParticleDestroyTimer;
                                GetWorldTimerManager().SetTimer(ParticleDestroyTimer, [SpawnedEffect]()
                                    {
                                        SpawnedEffect->DestroyComponent(); // Zerstöre das Partikelsystem nach 2 Sekunden
                                    }, 2.0f, false);
                            }
                        }
                        EnemyCharacter->GetDMG(dmgThunder);// Beispielschaden von 10 Punkten
                    }
                }
            }
        }
    }

   
}

void APlayerCharacter::StartBlink()
{
    UNiagaraComponent* SSpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BlinkEffect, GetActorLocation(), GetActorRotation());

    // 7️⃣ Effekt nach 1 Sekunde zerstören, um Speicher zu sparen
    if (SSpawnedEffect)
    {
        FTimerHandle EffectDestroyHandle;
        GetWorldTimerManager().SetTimer(EffectDestroyHandle, FTimerDelegate::CreateLambda([SSpawnedEffect]()
            {
                if (SSpawnedEffect) SSpawnedEffect->DestroyComponent();
            }), 3.0f, false);
    }
    DoAnimation(rollMon);
    FTimerHandle explodeTimerHandle;
    GetWorldTimerManager().SetTimer(explodeTimerHandle, this, &APlayerCharacter::Blink, 0.5f, false);
}

void APlayerCharacter::StartAuto()
{
    if (ProjectileClass && !autoAttackB) {
      //  DoAnimation(autoMon);
        ShotBallLightning();
       // GetWorldTimerManager().SetTimer(autoTimerHandle, this, &APlayerCharacter::ShotBallLightning, 0.2f, false);
        FTimerHandle autotot;
        GetWorldTimerManager().SetTimer(autotot, this, &APlayerCharacter::LoopAuto, autoTimerC, true);
    }
}

void APlayerCharacter::StartThunder()
{
    if (!thunderB && mana_current >= manaCostThunder) {
        DoAnimation(thunderMon);
        FTimerHandle thunderTimerHandle;
        GetWorldTimerManager().SetTimer(thunderTimerHandle, this, &APlayerCharacter::DoThunder, 0.5f, false);
    }
}

void APlayerCharacter::StartExplode()
{
    if (explodeShot && !exlodeB && mana_current >= manaCostExplode) {
        DoAnimation(autoMon);
        FTimerHandle explodeTimerHandle;
        GetWorldTimerManager().SetTimer(explodeTimerHandle, this, &APlayerCharacter::ShotExplode, 0.5f, false);
    }
}

void APlayerCharacter::LoopAuto()
{
    if (ProjectileClass ) {
        // Animation ausführen
        DoAnimation(autoMon);

        // Der Angriff wird ausgeführt
        ShotBallLightning();
      //  GetWorldTimerManager().SetTimer(autoTimerHandle, this, &APlayerCharacter::ShotBallLightning, 0.2f, false);
    }
}

void APlayerCharacter::ShotExplode()
{
    if (explodeShot && !exlodeB && mana_current >= manaCostExplode) // Überprüfe, ob eine Projektil-Klasse zugewiesen wurde
    {
        DoAnimation(autoMon);
        mana_current -= manaCostExplode;
        exlodeB = true;
        // Erhalte die Spawn-Position und Rotation basierend auf dem Charakter
        FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f; // Spawn vor dem Charakter
        FRotator SpawnRotation = GetActorRotation();

        // Spawne das Projektil
        AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(explodeShot, SpawnLocation, SpawnRotation);

        if (SpawnedProjectile)
        {
            // Weitere Initialisierungen, falls erforderlich (z.B. Geschwindigkeit einstellen)
            // SpawnedProjectile->MoveSpeed = 1200.0f; // Beispiel: Geschwindigkeit setzen
        }
    }
}

void APlayerCharacter::CheckAllAbilityTImer(float deltaTime)
{
    if (autoAttackB) {
        cd_autoAttack -= deltaTime;
        if (cd_autoAttack <= 0) {
            autoAttackB = false;
        }
    }else{
        cd_autoAttack = autoTimerC;   
        ShotBallLightning();
    }

    if (thunderB) {
        cd_ThunderAttack -= deltaTime;
        if (cd_ThunderAttack <= 0) {
            thunderB = false;
        }
    }else {
        cd_ThunderAttack = thunderTimerC;
        thunderB = false;
    }

    if (exlodeB) {
        cd_ExplodeAttack -= deltaTime;
        if (cd_ExplodeAttack <= 0) {
            exlodeB = false;
        }
    }
    else {
        cd_ExplodeAttack = explodeTimerC;
        exlodeB = false;
    }
}

void APlayerCharacter::GetDMG(float dmgGet)
{
    if (life > 0) {
        life -= dmgGet;
    }
}

void APlayerCharacter::DoAllReg()
{
    if (life < maxlife) {
        life += life_reg;
        if (life > maxlife) {
            life = maxlife;
        }
    }

    if (mana_current < mana_max) {
        mana_current += mana_reg;
        if (mana_current > mana_max) {
            mana_current = mana_max;
        }
    }
}

void APlayerCharacter::Blink()
{
    if (!Controller || bIsRolling) return; // Falls der Charakter gerade rollt oder kein Controller existiert, abbrechen

    FVector StartLocation = GetActorLocation(); // Speichert die aktuelle Position

    // 1️⃣ Bewegungsrichtung bestimmen
    FVector BlinkDirection = GetVelocity().GetSafeNormal(); // Falls der Spieler läuft, diese Richtung nutzen

    // Falls der Spieler stillsteht, nutze stattdessen seine Blickrichtung
    if (BlinkDirection.IsNearlyZero())
    {
        BlinkDirection = GetActorForwardVector();
    }

    float BlinkDistance = 400.0f; // Distanz des Teleports

    // 2️⃣ Berechne die Blink-Zielposition basierend auf der Lauf-/Blickrichtung
    FVector TargetLocation = StartLocation + (BlinkDirection * BlinkDistance);

    // 3️⃣ Kollisionsprüfung: Falls Hindernis vor dem Blink-Ziel, stoppe vorher
    FHitResult Hit;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, TargetLocation, ECC_Visibility, QueryParams);
    if (bHit) // Falls eine Wand getroffen wird, teleportiere nur bis zum Hindernis
    {
        TargetLocation = Hit.Location;
    }

    // 4️⃣ Niagara-Effekt am Startpunkt spawnen (Blink-Start)
  //  UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BlinkEffect, StartLocation, GetActorRotation());
    /*UNiagaraComponent* SSpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BlinkEffect, GetActorLocation(), GetActorRotation());

    // 7️⃣ Effekt nach 1 Sekunde zerstören, um Speicher zu sparen
    if (SSpawnedEffect)
    {
        FTimerHandle EffectDestroyHandle;
        GetWorldTimerManager().SetTimer(EffectDestroyHandle, FTimerDelegate::CreateLambda([SSpawnedEffect]()
            {
                if (SSpawnedEffect) SSpawnedEffect->DestroyComponent();
            }), 3.0f, false);
    }*/
    // 5️⃣ Charakter teleportieren
    SetActorLocation(TargetLocation);

    // 6️⃣ Niagara-Effekt am Zielpunkt spawnen (Blink-Ende)
   UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BlinkEffect, TargetLocation, GetActorRotation());

    // 7️⃣ Effekt nach 1 Sekunde zerstören, um Speicher zu sparen
    if (SpawnedEffect)
    {
        FTimerHandle EffectDestroyHandle;
        GetWorldTimerManager().SetTimer(EffectDestroyHandle, FTimerDelegate::CreateLambda([SpawnedEffect]()
            {
                if (SpawnedEffect) SpawnedEffect->DestroyComponent();
            }), 3.0f, false);
    }
}

void APlayerCharacter::StartRoll()
{
    if (bIsRolling || !Controller) return; // Verhindert mehrfaches Rollen

    bIsRolling = true; // Setzt den Rollstatus

    // Speichert die aktuelle Charakterrotation vor dem Rollen
    FRotator SavedRotation = GetActorRotation();

    // Ermittelt die aktuelle Bewegungsrichtung des Charakters
    FVector RollDirection = GetVelocity().GetSafeNormal();

    // Falls der Charakter stillsteht, nutze die Blickrichtung
    if (RollDirection.IsNearlyZero())
    {
        RollDirection = GetActorForwardVector();
    }

    // Geschwindigkeit für die Rolle setzen
    float RollSpeed = 800.0f; // Geschwindigkeit anpassen
    float RollHeight = 200.0f; // Leichtes Anheben für den Sprungeffekt

    // Vorwärtsbewegung basierend auf der aktuellen Richtung
    FVector RollVelocity = RollDirection * RollSpeed + FVector(0, 0, RollHeight);

    // Charakter bewegen
   // LaunchCharacter(RollVelocity, true, true);

    // Deaktiviere automatische Drehung zur Bewegung während des Rollens
    //GetCharacterMovement()->bOrientRotationToMovement = false;
    SetActorLocation(RollVelocity);
    // Setzt die Rotation auf die aktuelle Laufrichtung, sodass der Charakter in diese Richtung rollt
    SetActorRotation(SavedRotation);

    // Füge eine lokale Rotation für den visuellen Effekt hinzu
    GetMesh()->AddLocalRotation(FRotator(1080, 0, 0)); // 3 Umdrehungen

    // Animation abspielen (falls vorhanden)
    /*
    if (RollAnimation)
    {
        PlayAnimMontage(RollAnimation);
    }
    */

    // Timer zum Beenden der Rolle setzen
    FTimerHandle RollTimerHandle;
    GetWorldTimerManager().SetTimer(RollTimerHandle, this, &APlayerCharacter::EndRoll, 1.5f, false);

}

void APlayerCharacter::DoRollAnim()
{
   // DoAnimation(rollMon);
    FTimerHandle RrollTimerHandle;
    GetWorldTimerManager().SetTimer(RrollTimerHandle, this, &APlayerCharacter::StartRoll, 0.5f, false);
}

void APlayerCharacter::EndRoll()
{
    bIsRolling = false;
}

void APlayerCharacter::DoAnimation(UAnimMontage* attackMontage)
{
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

void APlayerCharacter::GEtSkillPoint(float amount)
{
    skillpoint += amount;
}

void APlayerCharacter::RefreshPlayerStats()
{
    if (chestItem)
    {
        // Überprüfe, ob der Life-Wert im Item vorhanden ist
        float LifeValue = 0.f;
        life -= saveLifeStat;
        maxlife -= saveLifeStat;
        if (chestItem->ItemStats.Contains(EItemStatType::Life))
        {
            LifeValue = chestItem->ItemStats[EItemStatType::Life];
            saveLifeStat = LifeValue;
        }

        // Aktualisiere den Player's Life-Wert
        maxlife += LifeValue;
        life += LifeValue;
       // maxlife = life;

        float dmgValue = 0.0f;
        dmgAuto -= saveDmgStat;
        dmgThunder -= saveDmgStat;
        dmgExplode -= saveDmgStat;
        if (chestItem->ItemStats.Contains(EItemStatType::Dmg))
        {
            dmgValue = chestItem->ItemStats[EItemStatType::Dmg];
            saveDmgStat = dmgValue;
        }
        dmgAuto += dmgValue;
        dmgThunder += dmgValue;
        dmgExplode += dmgValue;
        // Optional: Logge den neuen Life-Wert
        UE_LOG(LogTemp, Warning, TEXT("Player Life updated: %f"), dmgAuto);

        float ManaValue = 0.f;
        mana_current -= saveManaStat;
        mana_max -= saveManaStat;
        if (chestItem->ItemStats.Contains(EItemStatType::Mana))
        {
            ManaValue = chestItem->ItemStats[EItemStatType::Mana];
            saveManaStat = ManaValue;
        }

        // Aktualisiere den Player's Life-Wert
        mana_max += ManaValue;
        mana_current += ManaValue;

        float LifeRegValue = 0.f;
        life_reg -= saveLifeRegStat;
        //mana_max -= saveLifeRegStat;
        if (chestItem->ItemStats.Contains(EItemStatType::Life))
        {
            LifeRegValue = chestItem->ItemStats[EItemStatType::Life];
            saveManaStat = LifeRegValue;
        }

        // Aktualisiere den Player's Life-Wert
        //mana_max += LifeRegValue;
        life_reg += LifeRegValue;

        float ManaRegValue = 0.f;
        mana_reg -= saveManaRegStat;
        //mana_max -= saveLifeRegStat;
        if (chestItem->ItemStats.Contains(EItemStatType::Life))
        {
            ManaRegValue = chestItem->ItemStats[EItemStatType::Life];
            saveManaRegStat = ManaRegValue;
        }

        // Aktualisiere den Player's Life-Wert
        //mana_max += LifeRegValue;
        mana_reg += ManaRegValue;
    }
}

void APlayerCharacter::RefreshItemStats(AItem* item)
{
    chestItem = item;
    RefreshPlayerStats();
}

void APlayerCharacter::GetChestItem()
{
    TArray<AActor*> FoundItems;

    // Hole alle Items, die den Tag "Chest" haben
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Chest"), FoundItems);

    // Iteriere über die gefundenen Items und führe eine Aktion aus
    for (AActor* FoundItem : FoundItems)
    {
        if (FoundItem)
        {
            // Casten zu deinem Item-Typ, wenn nötig
            chestItem = Cast<AItem>(FoundItem);
            if (chestItem)
            {
                // Hier kannst du den Wert des Items erhalten oder etwas anderes machen
              //  UE_LOG(LogTemp, Warning, TEXT("Gefundenes Chest-Item: %s"), *Item->GetName());
                RefreshPlayerStats();
            }
        }
    }
}
