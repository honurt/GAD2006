// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameState.h"
#include "NetBaseCharacter.h"
#include "NetPlayerState.h"
#include "NetGameMode.h"
#include "Net/UnrealNetwork.h"

ANetGameState::ANetGameState() :
	WinningPlayer(-1),
	GameTimer(30.0f),
	RemainingTime(GameTimer)
{

}

void ANetGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetGameState, WinningPlayer);
}

void ANetGameState::OnRep_Winner()
{
	if (WinningPlayer >= 0)
	{
		OnVictory();
	}
}

void ANetGameState::TriggerRestart_Implementation()
{
	TimerStart();
	OnRestart();
}

ANetPlayerState* ANetGameState::GetPlayerStateByIndex(int PlayerIndex)
{
	for (APlayerState* PS : PlayerArray)
	{
		ANetPlayerState* State = Cast<ANetPlayerState>(PS);
		if (State && State->PlayerIndex == PlayerIndex) return State;
	}

	return nullptr;
}

void ANetGameState::TimerStart()
{	
	RemainingTime = GameTimer;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ANetGameState::UpdateTimer, 1.0f, true);
}


void ANetGameState::TimerStop_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void ANetGameState::UpdateTimer()
{

	if (RemainingTime > 0)
	{
		RemainingTime -= 1.0f;
		UpdateTimerDisplay(RemainingTime);
	}
	else
	{
		ANetGameMode* GameMode = Cast<ANetGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->TimeFinish();
		}
	}
}

void ANetGameState::OnRep_RemainingTime()
{
	UpdateTimerDisplay(RemainingTime);
}


