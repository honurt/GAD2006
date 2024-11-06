// Copyright Epic Games, Inc. All Rights Reserved.

#include "LearningCPPGameMode.h"
#include "LearningCPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALearningCPPGameMode::ALearningCPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
