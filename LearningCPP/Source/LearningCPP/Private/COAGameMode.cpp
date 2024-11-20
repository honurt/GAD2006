// Fill out your copyright notice in the Description page of Project Settings.


#include "COAGameMode.h"

#include "COAGameState.h"
#include "Engine/Engine.h"

ACOAGameMode::ACOAGameMode()
{
	PointsToWin = 5;
}

void ACOAGameMode::OnTargetHit()
{
	 if(ACOAGameState* GS = Cast<ACOAGameState>(GameState))
	 {
		 GS->Points++;

	 	if(GS->Points >= PointsToWin)
	 	{
	 		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("You Won!"));

	 	}
	    else
	    {
	    	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("Score: %d"), GS->Points));
	    }
	 }
}
