// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGameManager.h"

#include "TileBase.h"
#include "GameFramework/PlayerInput.h"
#include "TilePlayerController.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
ATileGameManager::ATileGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GridSize = 100.f,	
	GridOffset = FVector(0, 0, 0.5f);

	MapExtendsInGrids = 25;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	GridSelection = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
	GridSelection->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Engine/BasicShapes/Plane.Plane"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> GridMaterial(TEXT("/Game/UI/MAT_GridSlot.MAT_GridSlot"));

	GridSelection->SetStaticMesh(PlaneMesh.Object);
	GridSelection->SetMaterial(0, GridMaterial.Object);
	GridSelection->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CurrentGrid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CurrentGrid"));
	CurrentGrid->SetupAttachment(GridSelection);
	CurrentGrid->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
}

// Called when the game starts or when spawned
void ATileGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (auto PlayerController = Cast<ATilePlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerController->GameManager = this;
	}
}

// Called every frame
void ATileGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileGameManager::OnActorInteraction(AActor* Actor, const FVector& Location)
{
	if (TileTypes.Num() == 0) return;

	FVector GridLoc = GridOffset;
	GridLoc.X += FMath::GridSnap(Location.X, GridSize);
	GridLoc.Y += FMath::GridSnap(Location.Y, GridSize);
	GridLoc.Z += Location.Z;

	UPlayerInput* Input = GetWorld()->GetFirstPlayerController()->PlayerInput;

	
	if (Input->WasJustPressed(EKeys::LeftMouseButton))
	{
		int GridX = GridLoc.X / GridSize + MapExtendsInGrids;
		int GridY = GridLoc.Y / GridSize + MapExtendsInGrids;

		if (GridX < 0 || GridY < 0 || GridX >= MapExtendsInGrids*2 || GridY >= MapExtendsInGrids*2) return;
		if(Map[GridX][GridY] != nullptr) return;

		if (TileTypes.IsValidIndex(CurrentTileIndex))
		{
			ATileBase* SelectedTile = TileTypes[CurrentTileIndex];
			Map[GridX][GridY] = SelectedTile;

			FTransform TileTransform(GridLoc + GridOffset);
			SelectedTile->InstancedMesh->AddInstance(SelectedTile->InstancedMesh->GetRelativeTransform() * TileTransform, true);
		}

		UE_LOG(LogTemp, Warning, TEXT("Hit: %s - %f, %f, %f"), Actor ? *Actor->GetActorLabel() : TEXT("None"),
			Location.X,
			Location.Y,
			Location.Z);
	}
	else if (Input->WasJustPressed(EKeys::MouseScrollDown))
	{
		CurrentTileIndex = (CurrentTileIndex + 1) % TileTypes.Num();
		UE_LOG(LogTemp, Warning, TEXT("TileType: %s"), *TileTypes[CurrentTileIndex]->GetActorLabel())

		UStaticMesh* SelectedMesh = TileTypes[CurrentTileIndex]->InstancedMesh->GetStaticMesh();
		FVector SelectedScale = TileTypes[CurrentTileIndex]->InstancedMesh->GetRelativeScale3D();
	
		CurrentGrid->SetStaticMesh(SelectedMesh);
		CurrentGrid->SetWorldScale3D(SelectedScale);

		// Apply the same relative transform
		//FTransform CurrentGridTransform = CurrentGrid->GetRelativeTransform();
		//CurrentGrid->SetRelativeTransform(CurrentGridTransform);
	}
	else if (Input->WasJustPressed(EKeys::MouseScrollUp))
	{
		if (TileTypes.IsValidIndex(CurrentTileIndex - 1)) CurrentTileIndex = (CurrentTileIndex - 1) % TileTypes.Num();
		
		UE_LOG(LogTemp, Warning, TEXT("TileType: %s"), *TileTypes[CurrentTileIndex]->GetActorLabel())
		CurrentGrid->SetStaticMesh(TileTypes[CurrentTileIndex]->InstancedMesh->GetStaticMesh());
	}
	else if (Input->WasJustPressed(EKeys::RightMouseButton))
	{
		GridSelection->AddRelativeRotation(FRotator(0, 90, 0));
		CurrentGrid->AddRelativeRotation(FRotator(0,90,0));
	}
	else
	{
		GridSelection->SetWorldLocation(GridLoc + GridOffset);
	}
}

