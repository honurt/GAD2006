// Fill out your copyright notice in the Description page of Project Settings.


#include "COAAvatar.h"

#include "COABullet.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

#include "Components/TimelineComponent.h"



#include "Kismet/KismetMathLibrary.h"



ACOAAvatar::ACOAAvatar()
{
	RunSpeed = 900.0f;
	Stamina = MaxStamina;
	bStaminaDrained = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->TargetArmLength = OriginalArmLength = 300.f;
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed =  40.0f;
	SpringArm->SocketOffset.Set(30.f, 60.f, 40.f);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);


	Timeline = CreateDefaultSubobject<UTimelineComponent>("Timeline");
	ZoomProgress.BindUFunction(this, "AdjustCam");
}

void ACOAAvatar::BeginPlay()
{
	Super::BeginPlay();
	
	Camera->bUsePawnControlRotation = false;
	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;

	if (CamCurve)
	{
		Timeline->AddInterpFloat(CamCurve, ZoomProgress, FName("Alpha"));
	}

	
	
}

void ACOAAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetCharacterMovement()->IsFalling() && bRunning == true)
	{
		UpdateMovementParams();
	}
	
	if (bRunning == true)
	{
		Stamina -= StaminaDrainRate * DeltaTime;
		Stamina = FMath::Clamp(Stamina, 0.0f, MaxStamina);

		if (Stamina <= 0)
		{
			bStaminaDrained = true;
			bRunning = false;
			UpdateMovementParams();
			Timeline->Reverse();
			
		}
	}
	else
	{
		if (Stamina < MaxStamina && !GetCharacterMovement()->IsFalling())
		{
			Stamina += StaminaGainRate * DeltaTime;
			Stamina = FMath::Clamp(Stamina, 0.0f, MaxStamina);

			if (bStaminaDrained == true && Stamina == MaxStamina)
			{
				bStaminaDrained = false;
			}
			
		}
		
		
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Stamina: %.f"), Stamina));
	
}

void ACOAAvatar::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	
	if(GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::LeftShift) && !bRunning && !bStaminaDrained)
	{
		
		UpdateMovementParams();
		Timeline->Play();
		
	}

}

void ACOAAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn",this,&ACharacter::AddControllerYawInput); //Uses Positive value of Mouse
	PlayerInputComponent->BindAxis("LookUp",this,&ACharacter::AddControllerPitchInput); //Uses Negative value of Mouse
	
	PlayerInputComponent->BindAxis("MoveRight",this,&ACOAAvatar::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward",this,&ACOAAvatar::MoveForward);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ACOAAvatar::JumpPressed);
	PlayerInputComponent->BindAction("Jump",IE_Released,this,&ACOAAvatar::JumpReleased);

	PlayerInputComponent->BindAction("Run",IE_Pressed,this,&ACOAAvatar::RunPressed);
	PlayerInputComponent->BindAction("Run",IE_Released,this,&ACOAAvatar::RunReleased);

	PlayerInputComponent->BindAction("Shoot",IE_Pressed,this,&ACOAAvatar::ShootPressed);	
}

void ACOAAvatar::ShootPressed()
{
	if (BulletClass)
	{
		FActorSpawnParameters spawnParameters;
		spawnParameters.Owner = this;
		spawnParameters.Instigator = this;
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		spawnParameters.bNoFail = true;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetActorForwardVector() * 100.f + GetActorLocation());
		SpawnTransform.SetRotation(GetViewRotation().Quaternion());
		SpawnTransform.SetScale3D(FVector(1, 1, 1));

		GetWorld()->SpawnActor<ACOABullet>(BulletClass, SpawnTransform, spawnParameters);
	}
}

void ACOAAvatar::AdjustCam(float value)
{
	SpringArm->TargetArmLength = (FMath::Lerp(OriginalArmLength, RunArmLength, value));
}


void ACOAAvatar::RunPressed()
{
	if(GetVelocity().Size() > 0 && GetCharacterMovement()->IsMovingOnGround())
	{
		
		if (bStaminaDrained == false)
		{
			UpdateMovementParams();
			Timeline->Play();
		}
	}
}

void ACOAAvatar::RunReleased()
{
	if (bRunning == true)
	{
		Timeline->Reverse();
		UpdateMovementParams();
	}
}

void ACOAAvatar::MoveForward(float Amount)
{
	FRotator ControlRotation = GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, Amount);
}

void ACOAAvatar::MoveRight(float Amount)
{
	FRotator ControlRotation = GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(ForwardDirection, Amount);
}

void ACOAAvatar::JumpPressed()
{
	bool bFound;
	InitVaultCheck(bFound);
	if (!bFound)
	{
		ACOAAvatar::Jump();
	}
	
}

void ACOAAvatar::JumpReleased()
{

}

void ACOAAvatar::UpdateMovementParams()
{
	if (bStaminaDrained)
	{
		
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		return;		
	}

	
	bRunning = !bRunning;
	GetCharacterMovement()->MaxWalkSpeed = bRunning ? RunSpeed : WalkSpeed;
}


void ACOAAvatar::InitVaultCheck(bool& VaultFound)
{
	FVector SettingLoc;
	FHitResult Hit;
	FVector StartPos = GetActorLocation();
	FVector EndPos = StartPos + GetActorForwardVector() * JumpCheckLength;
	GetWorld()->LineTraceSingleByChannel(Hit, StartPos,EndPos, ECollisionChannel::ECC_Visibility);

	DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red, true, 1,0,1.f);
	
	if(Hit.bBlockingHit)
	{
		SettingLoc = Hit.Location;
		
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("HIT"));
		VaultFound = true;
		
	}
	else
	{
		VaultFound = false;
	}

	if (VaultFound)
	{
		FHitResult VHit;

		
		FVector ToLoc = SettingLoc + UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::MakeRotFromX(Hit.Normal)) * -20.f;

		
		FVector UpLimit = ToLoc + FVector(0, 0, 300);  
		FVector DownLimit = ToLoc + FVector(0, 0, -5); 

		// Perform the vertical line trace
		GetWorld()->LineTraceSingleByChannel(VHit, UpLimit, DownLimit, ECollisionChannel::ECC_Visibility);

		// Draw the debug line
		DrawDebugLine(GetWorld(), UpLimit, DownLimit, FColor::Red, true, 1, 0, 1.f);

		
		if (VHit.bBlockingHit)
		{
			FVector FinalLocation = VHit.Location + FVector(0, 0, 60); 
			SetActorLocation(FinalLocation);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Vertical trace failed."));
		}
	}

}
