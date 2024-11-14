// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "COABaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "COAAvatar.generated.h"

/**
 * 
 */
UCLASS()
class LEARNINGCPP_API ACOAAvatar : public ACOABaseCharacter
{
	GENERATED_BODY()
	
public:

	ACOAAvatar();
		
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "COA")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "COA")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "COA")
	float RunSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "COA")
	bool bRunning;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "COA")
	float Stamina;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "COA")
	float MaxStamina = 100;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "COA")
	float StaminaDrainRate = 1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "COA")
	float StaminaGainRate = 2;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "COA")
	bool bStaminaDrained;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void ShootPressed();


private:
	void RunPressed();
	void RunReleased();
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void UpdateMovementParams();

protected:
	
	UPROPERTY(EditAnywhere, Category = "ShootingLogic")
	TSubclassOf<class ACOABullet> BulletClass;
};
