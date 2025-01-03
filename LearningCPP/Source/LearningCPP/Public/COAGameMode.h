// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "COAGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LEARNINGCPP_API ACOAGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	public:
	ACOAGameMode();

	void OnTargetHit();

	UPROPERTY(EditAnywhere, Category="Rules")
	int32 PointsToWin;
};
