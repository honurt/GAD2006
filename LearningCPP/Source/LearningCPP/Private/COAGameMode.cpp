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
	 		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, *FString::Printf(TEXT("Score: %d"), GS->Points));
	 		GEngine->AddOnScreenDebugMessage(-1, 12.0f, FColor::Yellow, TEXT("You Won!"));

	 	}
	    else
	    {
	    	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, *FString::Printf(TEXT("Score: %d"), GS->Points));
	    }
	 }
}
