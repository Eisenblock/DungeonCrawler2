#pragma once
#include "Item.h"
#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Projectile.h"
#include "PlayerCharacter.generated.h"



UCLASS()
class DUNGECRAWLER2_API APlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    APlayerCharacter();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Bewegung & Maussteuerung
    void MoveForward(float Value);
    void MoveRight(float Value);
    void RotateToMouse();
    void ShotBallLightning();
    void DoThunder();
    void StartBlink();
    void StartAuto();
    void StartThunder();
    void StartExplode();
    void LoopAuto();
    void ShotExplode();
    void CheckAllAbilityTImer(float deltaTime);
    void GetDMG(float dmgGet);
    void DoAllReg();
    void Blink();
    void StartRoll();
    void DoRollAnim();
    void EndRoll();
    void DoAnimation(UAnimMontage* attackMontage);
    void GEtSkillPoint(float amount);
    void RefreshPlayerStats();
    void RefreshItemStats(AItem* item);
    void GetChestItem();

    //Item Stats

private:

public:
 
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<class UUserWidget> m_cPlayerHUD;
    //Atribute
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =  "Movement")
    float life = 100.0f;
    float saveLifeStat = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float maxlife = 100.0f;
    UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveSpeed = 600.0f;

    float saveDmgStat = 0;
    float saveManaStat = 0;
    float saveLifeRegStat = 0;
    float saveManaRegStat = 0;


    UPROPERTY(VisibleAnywhere)
    FVector MovementInput;

    UPROPERTY(VisibleAnywhere)
    FVector LastDirection;
    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Animation")
    bool moveForward = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool moveBack = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool moceRight = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool moveLeft = false;
    UPROPERTY(EditAnywhere, Category = "Animation")
    class UAnimInstance* AnimInstance;

    UPROPERTY(EditAnywhere, Category = "Animation")
    class UAnimMontage* RunForwardAnim;

    UPROPERTY(EditAnywhere, Category = "Animation")
    class UAnimMontage* RunBackwardAnim;

    UPROPERTY(EditAnywhere, Category = "Animation")
    class UAnimMontage* RunSidewaysAnim;
    FTimerHandle autoTimerHandle;
    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimMontage* autoMon;
    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimMontage* thunderMon;
    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimMontage* rollMon;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* TopDownCameraComponent;

    /** Camera boom positioning the camera above the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    TSubclassOf<AProjectile> ProjectileClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    TSubclassOf<AProjectile> explodeShot;

    bool bIsRolling = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UNiagaraSystem* CleaveEffect;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UNiagaraSystem* BlinkEffect;

    //Abilitys
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilitys")
    float cd_autoAttack = 3.0f;
    bool autoAttackB = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilitys")
    float getManaAuto = 20.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilitys")
    float cd_ThunderAttack = 3.0f;
    bool thunderB = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilitys")
    float manaCostThunder = 20.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilitys")
    float cd_ExplodeAttack = 3.0f;
    bool exlodeB = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilitys")
    float manaCostExplode = 20.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilitys")
    float dmgAuto = 20.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilitys")
    float dmgThunder = 20.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilitys")
    float dmgExplode = 40.0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilitys")
    float mana_current;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilitys")
    float mana_max;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilitys")
    float mana_reg = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilitys")
    float life_reg = 0.0f;
    FTimerHandle TimerHandle_DoAllReg;
    AItem* chestItem;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilitys")
    float skillpoint = 0.0f;

    float autoTimerC = 0.0f;
    float explodeTimerC = 0.0f;
    float thunderTimerC = 0.0f;

};
