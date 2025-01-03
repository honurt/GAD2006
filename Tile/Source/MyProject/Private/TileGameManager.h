// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileGameManager.generated.h"

#define MAX_MAP_GRID_SIZE 100

class ATileBase;

UCLASS()
class ATileGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileGameManager();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> GridSelection;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> CurrentGrid;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnActorInteraction(AActor* Actor, const FVector &Location);
	
	UPROPERTY(EditAnywhere)
	int32 GridSize;

	UPROPERTY(EditAnywhere)
	FVector GridOffset;

	UPROPERTY(EditAnywhere)
	TArray<ATileBase*> TileTypes;

	UPROPERTY(EditAnywhere)
	int MapExtendsInGrids;

	int CurrentTileIndex;
	
	TObjectPtr<ATileBase> Map[MAX_MAP_GRID_SIZE][MAX_MAP_GRID_SIZE];

};
