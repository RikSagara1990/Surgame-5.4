// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnduranceComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FUpdateEndurance)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURGAME_API UEnduranceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnduranceComponent();
	
	void SetEndurance(float Value);
	void SetMaxEndurance(float Value);
	void SetFullEndurance();
	void Initialized(float CurrentValue, float MaxValue);
	float GetEnduranceInPercent() const;


	float GetEndurance() const { return CurrentEndurance; }

	FUpdateEndurance UpdateEndurance;

private:
	int CurrentEndurance = 0;
	int MaxEndurance = 0;
};
