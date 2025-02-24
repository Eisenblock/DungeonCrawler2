// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "PlayerCharacter.h"
#include "Item.generated.h"

class APlayerCharacter;

UENUM(BlueprintType)
enum class EItemStatType : uint8
{
	Life    UMETA(DisplayName = "Life"),
	Mana    UMETA(DisplayName = "Mana"),
	Dmg     UMETA(DisplayName = "Dmg"),
	lifeReg UMETA(DisplayName = "LifeReg"),
	manaREg UMETA(DisplayName = "ManaReg")
};

UCLASS()
class DUNGECRAWLER2_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Stats")
	TMap<EItemStatType, float> ItemStats;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float RollLifeforItem();
	UFUNCTION(BlueprintCallable, Category = "Stats")
	float RollDmgforItem();
	UFUNCTION(BlueprintCallable, Category = "Stats")
	float RollLifeRegforItem();
	UFUNCTION(BlueprintCallable, Category = "Stats")
	float RollManaRegforItem();
	UFUNCTION(BlueprintCallable, Category = "Stats")
	float RollManaforItem();

	// Methode zum Setzen eines Stats
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetItemStat(EItemStatType StatType, float Value);

	// Methode zum Abrufen eines Stats
	UFUNCTION(BlueprintCallable, Category = "Stats")
	FString GetItemStat(EItemStatType StatType) const;

	APlayerCharacter* character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Stats")
	bool whiteB = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Stats")
	bool blueB = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Stats")
	bool goldB = false;
	float tierStats = 0;
};
