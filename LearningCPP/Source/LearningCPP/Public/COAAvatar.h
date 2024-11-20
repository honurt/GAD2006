// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "COABaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
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
	float StaminaDrainRate = 25;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "COA")
	float StaminaGainRate = 20;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "COA")
	bool bStaminaDrained;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "COA")
	float JumpCheckLength;

	//Cam
	float OriginalArmLength;

	UPROPERTY(EditAnywhere, Category="COACamera")
	float RunArmLength;
	
	UPROPERTY(EditAnywhere, Category="COACamera")
	UCurveFloat* CamCurve;
	//
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Landed(const FHitResult& Hit) override;
	
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void ShootPressed();

	UFUNCTION()
	void AdjustCam(float value);

private:
	void RunPressed();
	void RunReleased();
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void JumpPressed();
	void JumpReleased();
	void UpdateMovementParams();

	void InitVaultCheck(bool& FoundVault);

	FOnTimelineFloat ZoomProgress;

private:
	UPROPERTY()
	UTimelineComponent* Timeline;
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "ShootingLogic")
	TSubclassOf<class ACOABullet> BulletClass;
};
