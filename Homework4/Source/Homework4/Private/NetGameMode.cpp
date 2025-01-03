// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameMode.h"
#include "NetBaseCharacter.h"
#include "EngineUtils.h"
#include "NetGameState.h"
#include "NetPlayerState.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "GameFramework/PlayerStart.h"



ANetGameMode::ANetGameMode() 
{
	DefaultPawnClass = ANetBaseCharacter::StaticClass();
	PlayerStateClass = ANetPlayerState::StaticClass();
	GameStateClass = ANetGameState::StaticClass();
}

void ANetGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(GetGameState<ANetGameState>()->PlayerArray.Num() > 0)
	{
		GetGameState<ANetGameState>()->TimerStart();
	}
}

AActor* ANetGameMode::GetPlayerStart(FString Name, int Index)
{
	FName PSName;
	if (Index < 0) PSName = *Name;
	else
	{
		PSName = *FString::Printf(TEXT("%s%d"), *Name, Index % 4);
	}

	for (TActorIterator<APlayerStart> It(GWorld); It; ++It)
	{
		if (APlayerStart* PS = Cast<APlayerStart>(*It))
		{
			if (PS->PlayerStartTag == PSName)
			{
				return *It;
			}
		}
	}

	return nullptr;
}

AActor* ANetGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	AActor* Start = AssignTeamAndPlayerStart(Player);
	return Start ? Start : Super::ChoosePlayerStart_Implementation(Player);
}

AActor* ANetGameMode::AssignTeamAndPlayerStart(AController* Player)
{
	AActor* Start = nullptr;
	ANetPlayerState* State = Player->GetPlayerState<ANetPlayerState>();
	if (State)
	{
		if (TotalGamesCount == 0)
		{
			
			State->TeamID = (TotalPlayerCount == 0) ? EPlayerTeam::Team_Blue : EPlayerTeam::Team_Red;
			State->PlayerIndex = TotalPlayerCount++;
			AllPlayers.Add(Cast<APlayerController>(Player));

			Start = GetTeamStartLocation(State->TeamID);
		}
		else
		{
			
			Start = GetNextTeamStartLocation(State);
		}

		if (Start)
		{
			UE_LOG(LogTemp, Warning, TEXT("Assigned Player %s to Team %d at %s"), *Player->GetHumanReadableName(), static_cast<int32>(State->TeamID), *Start->GetActorLocation().ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to assign Player %s to a team start!"), *Player->GetHumanReadableName());
		}
	}
	return Start;
}

AActor* ANetGameMode::GetTeamStartLocation(EPlayerTeam TeamID)
{
	
	if (TeamID == EPlayerTeam::Team_Blue)
	{
		return GetPlayerStart("Blue", -1);
	}
	else if (TeamID == EPlayerTeam::Team_Red)
	{
		return GetPlayerStart("Red", PlayerStartIndex++);
	}
	return nullptr;
}

AActor* ANetGameMode::GetNextTeamStartLocation(ANetPlayerState* State)
{
	ANetPlayerState* NetPlayerState = State;
	if (NetPlayerState)
	{
		EGameResults PreviousResult = NetPlayerState->Result;
		EPlayerTeam NewTeamID = (NetPlayerState->TeamID == EPlayerTeam::Team_Blue) ? EPlayerTeam::Team_Red : EPlayerTeam::Team_Blue;

		if (PreviousResult == EGameResults::Result_Won || PreviousResult == EGameResults::Result_Lost)
		{
			NetPlayerState->TeamID = NewTeamID;
			return GetTeamStartLocation(NewTeamID);
		}
	}
	return nullptr;
}


void ANetGameMode::AvatarsOverlapped(ANetAvatar* AvatarA, ANetAvatar* AvatarB)
{
	ANetGameState* GState = GetGameState<ANetGameState>();

	if (GState == nullptr || GState->WinningPlayer >= 0)
	{
		return;
	}

	ANetPlayerState* StateA = AvatarA->GetPlayerState<ANetPlayerState>();
	ANetPlayerState* StateB = AvatarB->GetPlayerState<ANetPlayerState>();
	if (StateA->TeamID == StateB->TeamID)
	{
		return;
	}

	if (StateA->TeamID == EPlayerTeam::Team_Red)
	{
		GState->WinningPlayer = StateA->PlayerIndex;
	}
	else
	{
		GState->WinningPlayer = StateB->PlayerIndex;
	}

	AvatarA->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	AvatarB->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	GState->OnVictory();

	for (APlayerController* Player : AllPlayers)
	{
		auto State = Player->GetPlayerState<ANetPlayerState>();

		if (State->TeamID == EPlayerTeam::Team_Blue)
		{
			State->Result = EGameResults::Result_Lost;
		}
		else
		{
			State->Result = EGameResults::Result_Won;
		}
	}
	FTimerHandle EndGameTimerHandle;
	GWorld->GetTimerManager().SetTimer(EndGameTimerHandle, this, &ANetGameMode::EndGame, 2.5f, false);
}

void ANetGameMode::EndGame()
{
	PlayerStartIndex = 0;
	TotalGamesCount++;
	GetGameState<ANetGameState>()->WinningPlayer = -1;
	for (APlayerController* Player : AllPlayers)
	{
		APawn* Pawn = Player->GetPawn();
		Player->UnPossess();
		Pawn->Destroy();
		Player->StartSpot.Reset();
		RestartPlayer(Player);
	}

	ANetGameState* GState = GetGameState<ANetGameState>();
	GState->TriggerRestart();
}

void ANetGameMode::TimeFinish_Implementation()
{
	
	ANetAvatar* AvatarA = nullptr;
	ANetAvatar* AvatarB = nullptr;

	for (APlayerController* Player : AllPlayers)
	{
		if (ANetAvatar* Avatar = GetPlayerAvatar(Player))
		{
			auto State = Avatar->GetPlayerState<ANetPlayerState>();
			if (State->TeamID == EPlayerTeam::Team_Red)
			{
				AvatarB = Avatar;
			}
			else
			{
				AvatarA = Avatar;
			}
		}
	}

	if (AvatarA && AvatarB)
	{
		SetWinningAvatar(AvatarA, AvatarB, true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AvatarA or AvatarB is nullptr in TimeFinish_Implementation!"));
	}
}

ANetAvatar* ANetGameMode::GetPlayerAvatar(APlayerController* Player) const
{
	if (!Player)
	{
		return nullptr;
	}

	return Cast<ANetAvatar>(Player->GetPawn());
}

void ANetGameMode::SetWinningAvatar(ANetAvatar* AAvatar, ANetAvatar* BAvatar, bool bRedWon)
{
	ANetGameState* GState = GetGameState<ANetGameState>();
	if (!IsValid(GState) || GState->WinningPlayer >= 0)
	{
		return;
	}

	GState->TimerStop();

	ANetPlayerState* StateA = AAvatar->GetPlayerState<ANetPlayerState>();
	ANetPlayerState* StateB = BAvatar->GetPlayerState<ANetPlayerState>();

	if (!IsValid(StateA) || !IsValid(StateB) || StateA->TeamID == StateB->TeamID)
	{
		return;
	}

	GState->WinningPlayer = (bRedWon == (StateA->TeamID == EPlayerTeam::Team_Red))
		? StateA->PlayerIndex
		: StateB->PlayerIndex;

	UpdateAvatarCollision(AAvatar, BAvatar);
	UpdatePlayerResults(bRedWon);

	GState->OnVictory();

	FTimerHandle EndGameTimerHandle;
	GWorld->GetTimerManager().SetTimer(EndGameTimerHandle, this, &ANetGameMode::EndGame, 2.5f, false);
}

void ANetGameMode::UpdateAvatarCollision(ANetAvatar* AAvatar, ANetAvatar* BAvatar) const
{
	if (AAvatar)
	{
		AAvatar->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}

	if (BAvatar)
	{
		BAvatar->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
}

void ANetGameMode::UpdatePlayerResults(bool bRedWon)
{
	for (APlayerController* Player : AllPlayers)
	{
		if (ANetPlayerState* State = Player->GetPlayerState<ANetPlayerState>())
		{
			State->Result = (State->TeamID == (bRedWon ? EPlayerTeam::Team_Red : EPlayerTeam::Team_Blue))
				? EGameResults::Result_Won
				: EGameResults::Result_Lost;
		}
	}
}

