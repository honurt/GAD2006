// Fill out your copyright notice in the Description page of Project Settings.


#include "COABullet.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

// Sets default values
ACOABullet::ACOABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
	SetRootComponent(BulletMesh);

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");
	Projectile->InitialSpeed=4000.f;
	Projectile->MaxSpeed=4000.f;

	
	
}

// Called when the game starts or when spawned
void ACOABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACOABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

