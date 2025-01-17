// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAxis("Move Forward / Backward", this, &AMyPlayerController::MoveForward);
	InputComponent->BindAxis("Move Right / Left", this, &AMyPlayerController::MoveSide);
	InputComponent->BindAxis("Look Up / Down Mouse", this, &AMyPlayerController::LookOnVertical);
	InputComponent->BindAxis("Turn Right / Left Mouse", this, &AMyPlayerController::LookOnHorizontal);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &AMyPlayerController::Sprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &AMyPlayerController::Walk);
	InputComponent->BindAction("Jump", IE_Released, this, &AMyPlayerController::Jump);
	InputComponent->BindAction("Action", IE_Released, this, &AMyPlayerController::Action);
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	AHumanBase* HumanBase = Cast<AHumanBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	SetHumanBase(HumanBase);
}

void AMyPlayerController::SetHumanBase(AHumanBase* HumanBase)
{
	CurrentHumanBase = HumanBase;
	CurrentHumanBase->MyPlayerController = this;
}

void AMyPlayerController::MoveForward(float Value)
{
	if(!CurrentHumanBase)
		return;
	CurrentHumanBase->Move(Value, 0);
}

void AMyPlayerController::MoveSide(float Value)
{
	if(!CurrentHumanBase)
		return;
	CurrentHumanBase->Move(0, Value);
}

void AMyPlayerController::LookOnVertical(float Value)
{
	if(!CurrentHumanBase)
		return;
	CurrentHumanBase->LookAt(Value, 0);
}

void AMyPlayerController::LookOnHorizontal(float Value)
{
	if(!CurrentHumanBase)
		return;
	CurrentHumanBase->LookAt(0, Value);
}

void AMyPlayerController::Sprint()
{
	if(!CurrentHumanBase)
		return;
	CurrentHumanBase->ServerValidateAndSetAction(EAction::Spring);
}

void AMyPlayerController::Walk()
{
	if(!CurrentHumanBase)
		return;
	CurrentHumanBase->ServerValidateAndSetAction(EAction::Walk);
}

void AMyPlayerController::Jump()
{
	if(!CurrentHumanBase)
		return;
	CurrentHumanBase->ServerValidateAndSetAction(EAction::Jump);
}

void AMyPlayerController::Action()
{
	if(!CurrentHumanBase)
		return;
	CurrentHumanBase->ServerValidateAndSetAction(EAction::Action);
}

