// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "COAGameState.generated.h"

/**
 * 
 */
UCLASS()
class LEARNINGCPP_API ACOAGameState : public AGameStateBase
{
	GENERATED_BODY()

	public:
	ACOAGameState();

	int32 Points;
	
};
