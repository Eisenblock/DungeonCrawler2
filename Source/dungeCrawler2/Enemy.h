#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Projectile.h"
#include <Components/WidgetComponent.h>
#include "Enemy.generated.h"

// Forward Declarations
class APlayerCharacter;
class UAnimMontage;
class AAIController;  // Hinzufügen der Forward-Deklaration für AIController

UCLASS()
class DUNGECRAWLER2_API AEnemy : public ACharacter
{
    GENERATED_BODY()

public:
    // Konstruktor
    AEnemy();

protected:
    // Wird beim BeginPlay aufgerufen
    virtual void BeginPlay() override;

public:
    // Wird jede Frame aufgerufen
    virtual void Tick(float DeltaTime) override;

    // SkeletalMesh des Gegners
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
    USkeletalMeshComponent* EnemyMesh;

    // Referenz zum Spieler (wird im Spiel durch den Tag gefunden)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
    APlayerCharacter* PlayerCharacter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float life = 100.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float life_max = 100.0f;
    UPROPERTY(EditAnywhere, Category = "Movement")
    float attackSpeed = 3.0f;
    // Abstand, den der Gegner zum Spieler halten soll
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
    float FollowDistance = 50.0f;

    // Die AnimMontage für den Angriff
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
    UAnimMontage* AttackMontage;

    // Findet den Spieler basierend auf einem Tag
    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void FindPlayer();

    // Lässt den Gegner dem Spieler folgen
    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void FollowPlayer(float DeltaTime);

    // Führt die Angriffsanimation aus
    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void Attack();
    void RangeAttack();
    void GetDMG(float dmgGet);
    void DoAnimation(UAnimMontage* attackMontage);
    void ResetCanAttack();
    void ResetSpeed();
    bool CHeckLineOfSideToPLayer();

    bool canAttack = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    TSubclassOf<AProjectile> ProjectileClass;
    //EnemyType
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
    bool RangeEnemy = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
    bool MeeleEnemy = false;
    //UI
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UWidgetComponent* HealthBarWidget;
private:
    // AIController-Referenz
    //AAIController* AIController;
};
