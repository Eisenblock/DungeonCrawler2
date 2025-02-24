#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "PlayerCharacter.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Projectile.generated.h"

//#include "Enemy.h"

// Vorwärtsdeklaration für die Niagara-Komponente
class UNiagaraComponent;
class USphereComponent;
class AEnemy;
class APlayerCharacter;

UCLASS()
class DUNGECRAWLER2_API AProjectile : public AActor
{
    GENERATED_BODY()

public:
    // Konstruktor
    AProjectile();

protected:
    // Wird beim BeginPlay aufgerufen
    virtual void BeginPlay() override;

public:
    // Wird jede Frame aufgerufen
    virtual void Tick(float DeltaTime) override;

    // Kollision und Mesh
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
    USphereComponent* CollisionComponent;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
    USphereComponent* CollisionComponentExplode;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
    UStaticMeshComponent* ProjectileMesh;

    // Niagara Component für visuelle Effekte
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
    UNiagaraComponent* NiagaraEffect;

    // Geschwindigkeit des Projektils (EditAnywhere ermöglicht es, diese im Editor zu ändern)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float MoveSpeed = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
    bool Enemy = false;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
    bool Player = false;

    // Überlappungsfunktion für das Projektile
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnOverlapBeginExplode(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    void ActivateExplodde();
    void DoDmgOverTime();
    void DEstroyObject();
    FTimerHandle dmgoverTimer;
    AEnemy* enemy;
    //AEnemy* enemyLost;
    UPROPERTY()
    TArray<AEnemy*> AffectedEnemies;

    TMap<AEnemy*, FTimerHandle> EnemyTimers;
    //Projetile Type
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
    float lifetime = 8.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
    float explodeLifeTime = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
    bool overTimeProjetile = false;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
    bool autoProjetile = false;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
    bool explodeProjetile = false;
    bool bIsAffectedByDoT = false;
    APlayerCharacter* PlayerCharacter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UNiagaraSystem* CleaveEffect;

};
