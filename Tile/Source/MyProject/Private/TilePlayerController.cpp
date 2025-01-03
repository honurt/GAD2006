// Fill out your copyright notice in the Description page of Project Settings.


#include "TilePlayerController.h"
#include "PlayerInput.generated.h"
#include "TileGameManager.h"

void ATilePlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;
}

void ATilePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameManager == nullptr) return;

	FHitResult Hit;
	FVector WorldLocation, WorldDirection;

	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		if (GetWorld()->LineTraceSingleByChannel(Hit, WorldLocation, WorldLocation + WorldDirection * 50000.f, ECC_Visibility))
		{
			AActor* HitActor = Hit.GetActor();
			GameManager->OnActorInteraction(HitActor, Hit.Location);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Hit"));
		}
	}
}
