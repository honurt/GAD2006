// Fill out your copyright notice in the Description page of Project Settings.


#include "COAAvatar.h"

#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"


ACOAAvatar::ACOAAvatar()
{
	RunSpeed = 900.0f;
	Stamina = MaxStamina;
	bStaminaDrained = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	
}

void ACOAAvatar::BeginPlay()
{
	Super::BeginPlay();
	Camera->bUsePawnControlRotation = false;
	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
}

void ACOAAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRunning == true)
	{
		Stamina = Stamina - StaminaDrainRate;

		if (Stamina <= 0)
		{
			bStaminaDrained = true;
			bRunning = false;
			UpdateMovementParams();
			
		}
	}
	else
	{
		if (Stamina < MaxStamina)
		{
			Stamina += StaminaGainRate;

			if (bStaminaDrained == true && Stamina == MaxStamina)
			{
				bStaminaDrained = false;
			}
			
		}
		
		
	}
	
}

void ACOAAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn",this,&ACharacter::AddControllerYawInput); //Uses Positive value of Mouse
	PlayerInputComponent->BindAxis("LookUp",this,&ACharacter::AddControllerPitchInput); //Uses Negative value of Mouse
	
	PlayerInputComponent->BindAxis("MoveRight",this,&ACOAAvatar::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward",this,&ACOAAvatar::MoveForward);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ACOAAvatar::Jump);
	PlayerInputComponent->BindAction("Jump",IE_Released,this,&ACOAAvatar::StopJumping);

	PlayerInputComponent->BindAction("Run",IE_Pressed,this,&ACOAAvatar::RunPressed);
	PlayerInputComponent->BindAction("Run",IE_Released,this,&ACOAAvatar::RunReleased);
}

void ACOAAvatar::RunPressed()
{
	if (bStaminaDrained == false)
	{
		UpdateMovementParams();
	}
}

void ACOAAvatar::RunReleased()
{
	if (bRunning == true)
	{
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

void ACOAAvatar::UpdateMovementParams()
{
	if (bStaminaDrained == false)
	{
		if (bRunning == true)
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			bRunning = false;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
			bRunning = true;
		}
		
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	
}
