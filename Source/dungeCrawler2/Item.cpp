 // Fill out your copyright notice in the Description page of Project Settings.
 

#include "Item.h"
#include "PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>



// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> FoundItems;

	// Hole alle Items, die den Tag "Chest" haben
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), FoundItems);
	character = Cast<APlayerCharacter>(FoundItems[0]);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AItem::RollLifeforItem()
{
	float lifeStat;

	
		if (whiteB) {
			int RandomNumber = FMath::RandRange(0, 10); // Zufallszahl zwischen 0 und 10

			if (RandomNumber <= 2) {
				lifeStat = FMath::RandRange(60, 140); // Zufallszahl zwischen 14 und 60
			}
			else {
				lifeStat = FMath::RandRange(30, 60); // Zufallszahl zwischen 30 und 60
			}

			UE_LOG(LogTemp, Warning, TEXT("Zufallszahl: %d"), RandomNumber);
			UE_LOG(LogTemp, Warning, TEXT("Leben: %f"), lifeStat);
		}

		if (blueB) {
			int RandomNumber = FMath::RandRange(0, 10); // Zufallszahl zwischen 0 und 10

			if (RandomNumber <= 2) {
				lifeStat = FMath::RandRange(140, 240); // Zufallszahl zwischen 14 und 60
			}
			else if(RandomNumber >=2 && RandomNumber <=4) {
				lifeStat = FMath::RandRange(30, 60); // Zufallszahl zwischen 30 und 60
			}
			else if (RandomNumber >= 4 && RandomNumber <= 10) {
				lifeStat = FMath::RandRange(60, 140); // Zufallszahl zwischen 30 und 60
			}


			UE_LOG(LogTemp, Warning, TEXT("Zufallszahl: %d"), RandomNumber);
			UE_LOG(LogTemp, Warning, TEXT("Leben: %f"), lifeStat);
		}

		if (goldB) {
			int RandomNumber = FMath::RandRange(0, 10); // Zufallszahl zwischen 0 und 10

			if (RandomNumber <= 5) {
				lifeStat = FMath::RandRange(140, 240); // Zufallszahl zwischen 14 und 60
			}
			else {
				lifeStat = FMath::RandRange(60, 140); // Zufallszahl zwischen 30 und 60
			}


			UE_LOG(LogTemp, Warning, TEXT("Zufallszahl: %d"), RandomNumber);
			UE_LOG(LogTemp, Warning, TEXT("Leben: %f"), lifeStat);
		}

		lifeStat = FMath::FloorToFloat(lifeStat);


	return lifeStat;
}

float AItem::RollDmgforItem()
{
	float dmgStat;


	if (whiteB) {
		int RandomNumber = FMath::RandRange(0, 10); // Zufallszahl zwischen 0 und 10

		if (RandomNumber <= 2) {
			dmgStat = FMath::RandRange(20, 60); // Zufallszahl zwischen 14 und 60
		}
		else {
			dmgStat = FMath::RandRange(5, 20); // Zufallszahl zwischen 30 und 60
		}

		UE_LOG(LogTemp, Warning, TEXT("Zufallszahl: %d"), RandomNumber);
		UE_LOG(LogTemp, Warning, TEXT("Leben: %f"), dmgStat);
	}

	if (blueB) {
		int RandomNumber = FMath::RandRange(0, 10); // Zufallszahl zwischen 0 und 10

		if (RandomNumber <= 2) {
			dmgStat = FMath::RandRange(60, 120); // Zufallszahl zwischen 14 und 60
		}
		else if (RandomNumber >= 2 && RandomNumber <= 4) {
			dmgStat = FMath::RandRange(5, 20); // Zufallszahl zwischen 30 und 60
		}
		else if (RandomNumber >= 4 && RandomNumber <= 10) {
			dmgStat = FMath::RandRange(20, 60); // Zufallszahl zwischen 30 und 60
		}


		UE_LOG(LogTemp, Warning, TEXT("Zufallszahl: %d"), RandomNumber);
		UE_LOG(LogTemp, Warning, TEXT("Leben: %f"), dmgStat);
	}

	if (goldB) {
		int RandomNumber = FMath::RandRange(0, 10); // Zufallszahl zwischen 0 und 10

		if (RandomNumber <= 5) {
			dmgStat = FMath::RandRange(60, 120); // Zufallszahl zwischen 14 und 60
		}
		else {
			dmgStat = FMath::RandRange(20, 60); // Zufallszahl zwischen 30 und 60
		}


		UE_LOG(LogTemp, Warning, TEXT("Zufallszahl: %d"), RandomNumber);
		UE_LOG(LogTemp, Warning, TEXT("Leben: %f"), dmgStat);
	}

	dmgStat = FMath::FloorToInt(dmgStat);

	return dmgStat;
}

float AItem::RollLifeRegforItem()
{
	float lifeRegStat;


	if (whiteB) {
		int RandomNumber = FMath::RandRange(0, 10); // Zufallszahl zwischen 0 und 10

		if (RandomNumber <= 2) {
			lifeRegStat = FMath::RandRange(2, 4); // Zufallszahl zwischen 14 und 60
		}
		else {
			lifeRegStat = FMath::RandRange(1, 2); // Zufallszahl zwischen 30 und 60
		}

		UE_LOG(LogTemp, Warning, TEXT("Zufallszahl: %d"), RandomNumber);
		UE_LOG(LogTemp, Warning, TEXT("Leben: %f"), lifeRegStat);
	}

	if (blueB) {
		int RandomNumber = FMath::RandRange(0, 10); // Zufallszahl zwischen 0 und 10

		if (RandomNumber <= 2) {
			lifeRegStat = FMath::RandRange(4, 8); // Zufallszahl zwischen 14 und 60
		}
		else if (RandomNumber >= 2 && RandomNumber <= 4) {
			lifeRegStat = FMath::RandRange(1, 2); // Zufallszahl zwischen 30 und 60
		}
		else if (RandomNumber >= 4 && RandomNumber <= 10) {
			lifeRegStat = FMath::RandRange(2, 4); // Zufallszahl zwischen 30 und 60
		}


		UE_LOG(LogTemp, Warning, TEXT("Zufallszahl: %d"), RandomNumber);
		UE_LOG(LogTemp, Warning, TEXT("Leben: %f"), lifeRegStat);
	}

	if (goldB) {
		int RandomNumber = FMath::RandRange(0, 10); // Zufallszahl zwischen 0 und 10

		if (RandomNumber <= 5) {
			lifeRegStat = FMath::RandRange(2, 4); // Zufallszahl zwischen 14 und 60
		}
		else {
			lifeRegStat = FMath::RandRange(4, 8); // Zufallszahl zwischen 30 und 60
		}


		UE_LOG(LogTemp, Warning, TEXT("Zufallszahl: %d"), RandomNumber);
		UE_LOG(LogTemp, Warning, TEXT("Leben: %f"), lifeRegStat);
	}

	return lifeRegStat;
}

float AItem::RollManaRegforItem()
{
	float manaRegStat;


	if (whiteB) {
		int RandomNumber = FMath::RandRange(0, 10); // Zufallszahl zwischen 0 und 10

		if (RandomNumber <= 2) {
			manaRegStat = FMath::RandRange(2, 4); // Zufallszahl zwischen 14 und 60
		}
		else {
			manaRegStat = FMath::RandRange(1, 2); // Zufallszahl zwischen 30 und 60
		}

		UE_LOG(LogTemp, Warning, TEXT("Zufallszahl: %d"), RandomNumber);
		UE_LOG(LogTemp, Warning, TEXT("Leben: %f"), manaRegStat);
	}

	if (blueB) {
		int RandomNumber = FMath::RandRange(0, 10); // Zufallszahl zwischen 0 und 10

		if (RandomNumber <= 2) {
			manaRegStat = FMath::RandRange(4, 8); // Zufallszahl zwischen 14 und 60
		}
		else if (RandomNumber >= 2 && RandomNumber <= 4) {
			manaRegStat = FMath::RandRange(1, 2); // Zufallszahl zwischen 30 und 60
		}
		else if (RandomNumber >= 4 && RandomNumber <= 10) {
			manaRegStat = FMath::RandRange(2, 4); // Zufallszahl zwischen 30 und 60
		}


		UE_LOG(LogTemp, Warning, TEXT("Zufallszahl: %d"), RandomNumber);
		UE_LOG(LogTemp, Warning, TEXT("Leben: %f"), manaRegStat);
	}

	if (goldB) {
		int RandomNumber = FMath::RandRange(0, 10); // Zufallszahl zwischen 0 und 10

		if (RandomNumber <= 5) {
			manaRegStat = FMath::RandRange(2, 4); // Zufallszahl zwischen 14 und 60
		}
		else {
			manaRegStat = FMath::RandRange(4, 8); // Zufallszahl zwischen 30 und 60
		}


		UE_LOG(LogTemp, Warning, TEXT("Zufallszahl: %d"), RandomNumber);
		UE_LOG(LogTemp, Warning, TEXT("Leben: %f"), manaRegStat);
	}

	return manaRegStat;
}

float AItem::RollManaforItem()
{
	float manaStat;


	if (whiteB) {
		int RandomNumber = FMath::RandRange(0, 10); // Zufallszahl zwischen 0 und 10

		if (RandomNumber <= 2) {
			manaStat = FMath::RandRange(20, 60); // Zufallszahl zwischen 14 und 60
		}
		else {
			manaStat = FMath::RandRange(5, 20); // Zufallszahl zwischen 30 und 60
		}

		UE_LOG(LogTemp, Warning, TEXT("Zufallszahl: %d"), RandomNumber);
		UE_LOG(LogTemp, Warning, TEXT("Leben: %f"), manaStat);
	}

	if (blueB) {
		int RandomNumber = FMath::RandRange(0, 10); // Zufallszahl zwischen 0 und 10

		if (RandomNumber <= 2) {
			manaStat = FMath::RandRange(60, 120); // Zufallszahl zwischen 14 und 60
		}
		else if (RandomNumber >= 2 && RandomNumber <= 4) {
			manaStat = FMath::RandRange(5, 20); // Zufallszahl zwischen 30 und 60
		}
		else if (RandomNumber >= 4 && RandomNumber <= 10) {
			manaStat = FMath::RandRange(20, 60); // Zufallszahl zwischen 30 und 60
		}


		UE_LOG(LogTemp, Warning, TEXT("Zufallszahl: %d"), RandomNumber);
		UE_LOG(LogTemp, Warning, TEXT("Leben: %f"), manaStat);
	}

	if (goldB) {
		int RandomNumber = FMath::RandRange(0, 10); // Zufallszahl zwischen 0 und 10

		if (RandomNumber <= 5) {
			manaStat = FMath::RandRange(60, 120); // Zufallszahl zwischen 14 und 60
		}
		else {
			manaStat = FMath::RandRange(20, 60); // Zufallszahl zwischen 30 und 60
		}


		UE_LOG(LogTemp, Warning, TEXT("Zufallszahl: %d"), RandomNumber);
		UE_LOG(LogTemp, Warning, TEXT("Leben: %f"), manaStat);
	}

	return manaStat;
}

void AItem::SetItemStat(EItemStatType StatType, float Value)
{
	ItemStats.Add(StatType, Value);
	character->RefreshItemStats(this);
	//character->GetChestItem();
	//character->RefreshPlayerStats();
	UE_LOG(LogTemp, Warning, TEXT("Updated Stat - %s: %f"), *UEnum::GetValueAsString(StatType), ItemStats[StatType]);
}

FString AItem::GetItemStat(EItemStatType StatType) const
{
	FString StatsText;
	for (const TPair<EItemStatType, float>& StatPair : ItemStats)
	{
		FString StatName = UEnum::GetValueAsString(StatPair.Key);
		StatsText += FString::Printf(TEXT("%s: %.2f\n"), *StatName, StatPair.Value);
	}
	return StatsText;
}
