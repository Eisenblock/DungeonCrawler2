#include "Spawner.h"
#include "Enemy.h"
#include "Engine/World.h"
#include "AIController.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawner::ASpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    // Standardwerte setzen
    SpawnInterval = 5.0f;
    StartDelay = 2.0f;
    bIsSpawningActive = true;
    killGot = 0;
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
    Super::BeginPlay();

    // Alle Spawnpunkte mit dem Tag "SpawnPoint" sammeln
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SpawnPoint"), SpawnPoints);

    // Timer starten, um Gegner zu spawnen
    if (bIsSpawningActive && SpawnPoints.Num() > 0)
    {
        GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ASpawner::SpawnEnemy, SpawnInterval, true);
    }
}

void ASpawner::SpawnEnemy()
{
   // if (!bIsSpawningActive || !EnemyMage || SpawnPoints.Num() == 0) return;
    if (neededKills >= killGot) {
        killGot += 1;
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        // if (!PlayerController) return;

        APawn* PlayerPawn = PlayerController->GetPawn();
        //if (!PlayerPawn) return;

        FVector PlayerLocation = PlayerPawn->GetActorLocation();

        // Zufälligen gültigen Spawnpunkt finden
        FVector SpawnLocation;
        do {
            int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
            SpawnLocation = SpawnPoints[RandomIndex]->GetActorLocation();
        } while (FVector::Dist(SpawnLocation, PlayerLocation) < 500.0f);

        FRotator SpawnRotation = FRotator::ZeroRotator;

        // Gegner spawnen
        AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyMeele, SpawnLocation, SpawnRotation);

        if (SpawnedEnemy)
        {
            // Standard AIController verwenden
            AAIController* AIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass(), SpawnedEnemy->GetActorLocation(), SpawnedEnemy->GetActorRotation());

            if (AIController)
            {
                // Feind dem AIController zuweisen
                AIController->Possess(SpawnedEnemy);
            }

        }
    }
    else {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        // if (!PlayerController) return;

        APawn* PlayerPawn = PlayerController->GetPawn();
        //if (!PlayerPawn) return;

        FVector PlayerLocation = PlayerPawn->GetActorLocation();

        // Zufälligen gültigen Spawnpunkt finden
        FVector SpawnLocation;
        do {
            int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
            SpawnLocation = SpawnPoints[RandomIndex]->GetActorLocation();
        } while (FVector::Dist(SpawnLocation, PlayerLocation) < 500.0f);

        FRotator SpawnRotation = FRotator::ZeroRotator;


        if (enemyType == 0) {
            enemyType = 1;
            // Gegner spawnen
            AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyMeele, SpawnLocation, SpawnRotation);

            if (SpawnedEnemy)
            {
                // Standard AIController verwenden
                AAIController* AIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass(), SpawnedEnemy->GetActorLocation(), SpawnedEnemy->GetActorRotation());

                if (AIController)
                {
                    // Feind dem AIController zuweisen
                    AIController->Possess(SpawnedEnemy);
                }
            }
        }
        else {
            enemyType = 0;
            // Gegner spawnen
            AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyMage, SpawnLocation, SpawnRotation);

            if (SpawnedEnemy)
            {
                // Standard AIController verwenden
                AAIController* AIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass(), SpawnedEnemy->GetActorLocation(), SpawnedEnemy->GetActorRotation());

                if (AIController)
                {
                    // Feind dem AIController zuweisen
                    AIController->Possess(SpawnedEnemy);
                }
            }
        }
    }
}