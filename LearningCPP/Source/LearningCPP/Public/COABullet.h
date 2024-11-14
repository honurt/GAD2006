// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "COABullet.generated.h"

UCLASS()
class LEARNINGCPP_API ACOABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACOABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category="Components")
	UProjectileMovementComponent* Projectile;

	UPROPERTY(EditAnywhere, Category="Components")
	UStaticMeshComponent* BulletMesh;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
	
};
