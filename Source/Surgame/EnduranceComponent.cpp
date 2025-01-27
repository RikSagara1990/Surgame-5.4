// Fill out your copyright notice in the Description page of Project Settings.


#include "EnduranceComponent.h"

// Sets default values for this component's properties
UEnduranceComponent::UEnduranceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UEnduranceComponent::SetEndurance(float Value)
{
	CurrentEndurance = Value; 
	if (CurrentEndurance <= 0)
	{
		CurrentEndurance = 0;
	}
	if (CurrentEndurance > MaxEndurance)
		CurrentEndurance = MaxEndurance;

	if(UpdateEndurance.IsBound())
		UpdateEndurance.Broadcast();
}

void UEnduranceComponent::SetMaxEndurance(float Value)
{
	MaxEndurance = Value; 
	if(UpdateEndurance.IsBound())
		UpdateEndurance.Broadcast();
}

void UEnduranceComponent::SetFullEndurance()
{
	SetEndurance(MaxEndurance);

	if(UpdateEndurance.IsBound())
		UpdateEndurance.Broadcast();
}

void UEnduranceComponent::Initialized(float CurrentValue, float MaxValue)
{
	CurrentEndurance = CurrentValue;
	MaxEndurance = MaxValue;
	
	if(UpdateEndurance.IsBound())
		UpdateEndurance.Broadcast();
}

float UEnduranceComponent::GetEnduranceInPercent() const
{
	const float health = static_cast<float>(CurrentEndurance) / static_cast<float>(MaxEndurance);
	
	return health;
}


