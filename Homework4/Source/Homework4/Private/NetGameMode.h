// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetAvatar.h"
#include "NetPlayerState.h"
#include "GameFramework/GameModeBase.h"
#include "NetGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ANetGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	ANetGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	UFUNCTION(BlueprintCallable)
	void AvatarsOverlapped(ANetAvatar* AvatarA, ANetAvatar* AvatarB);

	UFUNCTION(BlueprintCallable)
	void EndGame();

	UFUNCTION(NetMulticast, Reliable)
	void TimeFinish();

	ANetAvatar* GetPlayerAvatar(APlayerController* Player) const;

	UFUNCTION()
	void SetWinningAvatar(ANetAvatar* AAvatar, ANetAvatar* BAvatar, bool bRedWon);

	void UpdateAvatarCollision(ANetAvatar* AAvatar, ANetAvatar* BAvatar) const;

	void UpdatePlayerResults(bool bRedWon);

private:
	int TotalPlayerCount;
	int TotalGamesCount;
	int PlayerStartIndex;

	UPROPERTY()
	TArray<APlayerController*> AllPlayers;
	
	AActor* GetPlayerStart(FString Name, int Index);

	AActor* AssignTeamAndPlayerStart(AController* Player);
	AActor* GetTeamStartLocation(EPlayerTeam TeamID);
	AActor* GetNextTeamStartLocation(ANetPlayerState* State);
};
