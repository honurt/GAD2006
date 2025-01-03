// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NetGameInstance.generated.h"	

/**
 * 
 */

UENUM(BlueprintType)
enum class EPlayerTeam : uint8
{
	Team_Unknown,
	Team_Blue,
	Team_Red,
};

UENUM(BlueprintType)
enum class EGameResults : uint8
{
	Result_Undefined,
	Result_Lost,
	Result_Won,
};

USTRUCT(BlueprintType)
struct FSPlayerInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Nickname;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CustomizationData;

	UPROPERTY(BlueprintReadOnly)
	EPlayerTeam TeamID;
};

UCLASS()
class UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable)
	void Host(FString MapName, FSPlayerInfo PlayerInfo);

	UFUNCTION(BlueprintCallable)
	void Join(FString Address, FSPlayerInfo PlayerInfo);

	UPROPERTY(BlueprintReadWrite)
	FSPlayerInfo PlayerInfo;
};
