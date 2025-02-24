#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class AEnemy;

UCLASS()
class DUNGECRAWLER2_API ASpawner : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASpawner();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Funktion zum Spawnen von Gegnern
    UFUNCTION()
    void SpawnEnemy();

public:
    // Gegnerklasse, die gespawnt wird (in Blueprints einstellbar)
    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<AEnemy> EnemyMage;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<AEnemy> EnemyMeele;
    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<AEnemy> EnemyMeele2;


    // Zeit zwischen den Spawns (in Blueprints einstellbar)
    UPROPERTY(EditAnywhere, Category = "Spawning")
    float SpawnInterval;
    UPROPERTY(EditAnywhere, Category = "Spawning")
    float waveNumber = 0.0f;

    // Verzögerung, bevor das Spawnen beginnt (in Blueprints einstellbar)
    UPROPERTY(EditAnywhere, Category = "Spawning")
    float StartDelay;

    // Ob das Spawning aktiv ist (in Blueprints steuerbar)
    UPROPERTY(EditAnywhere, Category = "Spawning")
    bool bIsSpawningActive;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    float neededKills = 20.0f;
    float killGot = 0.0f;
    //Meele 0
    //Mage 1
    //Meele more Life 2
    int enemyType = 0;

    TArray<AActor*> SpawnPoints;

private:
    // Timer-Handle für den Spawn-Mechanismus
    FTimerHandle SpawnTimerHandle;
};
