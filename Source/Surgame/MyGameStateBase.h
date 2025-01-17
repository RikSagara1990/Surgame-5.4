// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"
/**
 * 
 */
UCLASS()
class SURGAME_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputValue")
	float TurnRateGamepad = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSettingsValue")
	float EnduranceRecoveryDelay = 3;
	
	TMap<EAction, int> ActionEnduranceCost
	{
		{EAction::None, 0},
		{EAction::Walk, 0},
		{EAction::Spring, 4},
		{EAction::Jump, 10},
		{EAction::Action, 10},
		{EAction::Attack, 10}
	};

public:
	static bool IsActionHands(EAction Action);
};
