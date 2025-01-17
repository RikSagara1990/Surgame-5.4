// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HumanBase.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SURGAME_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "PlayerComponents")
	AHumanBase* CurrentHumanBase;

	void SetHumanBase(AHumanBase* HumanBase);
	void MoveForward(float Value);
	void MoveSide(float Value);
	void LookOnVertical(float Value);
	void LookOnHorizontal(float Value);
	void Sprint();
	void Walk();
	void Jump();
	void Action();
};
