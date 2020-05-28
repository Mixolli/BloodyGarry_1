// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BGCharacterBase.h"
#include "GameFramework\SpringArmComponent.h"
#include "Camera\CameraComponent.h"
#include "Components\StaticMeshComponent.h"
#include "Components\InputComponent.h"
#include "GameFramework\Controller.h"
#include "Runtime\Engine\Public\TimerManager.h"
#include "GameFramework\CharacterMovementComponent.h"

// Sets default values
ABGCharacterBase::ABGCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	MeshComp->SetupAttachment(RootComponent);

	//BaseTurnRate = 45.0f;
	//BaseLookUpRate = 45.0f;

	//Dash mechanic from "Nitrogen" https://www.youtube.com/watch?v=tSBepXvgFlA
	CanDash = true;
	DashDistance = 5000.f;
	DashCooldown = 1.f;
	DashStop = 0.1f;
}


void ABGCharacterBase::MoveForward(float value)
{
	if ((Controller) && (value != 0.0f))
	{
		//const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		//const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//AddMovementInput(Direction, value);
		AddMovementInput(FVector(1,0,0), value);
	}
}

void ABGCharacterBase::MoveRight(float value)
{
	if ((Controller) && (value != 0.0f))
	{
		//const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);

		//const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		//AddMovementInput(Direction, value);
		AddMovementInput(FVector(0, 1, 0), value);
	}
}

void ABGCharacterBase::TurnAtRate(float value)
{
	AddControllerYawInput(value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABGCharacterBase::LookUpAtRate(float value)
{
	AddControllerPitchInput(value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


//Dash mechanic from "Nitrogen" https://www.youtube.com/watch?v=tSBepXvgFlA
void ABGCharacterBase::Dash()
{
	if (CanDash)
	{
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		LaunchCharacter(FVector(GetActorForwardVector().X, GetActorForwardVector().Y,0).GetSafeNormal() * DashDistance, true, true);
		CanDash = false;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABGCharacterBase::StopDashing, DashStop, false);
	}
}

void ABGCharacterBase::StopDashing()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABGCharacterBase::ResetDash, DashCooldown, false);
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;

}

void ABGCharacterBase::ResetDash()
{
	CanDash = true;
}



// Called every frame
void ABGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABGCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ABGCharacterBase::Dash);
	

	PlayerInputComponent->BindAxis("MoveForward", this, &ABGCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABGCharacterBase::MoveRight);

	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABGCharacterBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABGCharacterBase::LookUpAtRate);
}

