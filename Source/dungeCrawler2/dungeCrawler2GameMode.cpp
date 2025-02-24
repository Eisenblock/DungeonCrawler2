// Copyright Epic Games, Inc. All Rights Reserved.

#include "dungeCrawler2GameMode.h"
#include "dungeCrawler2PlayerController.h"
#include "dungeCrawler2Character.h"
#include "UObject/ConstructorHelpers.h"

AdungeCrawler2GameMode::AdungeCrawler2GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AdungeCrawler2PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	//static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	//if(PlayerControllerBPClass.Class != NULL)
	//{
		//PlayerControllerClass = PlayerControllerBPClass.Class;
	//}
}