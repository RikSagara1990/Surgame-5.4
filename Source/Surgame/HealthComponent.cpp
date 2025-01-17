// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UHealthComponent::SetHealth_Implementation(float Value)
{
	CurrentHealth = Value; 
	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
	}
	if (CurrentHealth > MaxHealth)
		CurrentHealth = MaxHealth;

	if(UpdateHealth.IsBound())
		UpdateHealth.Broadcast();
}

void UHealthComponent::SetMaxHealth_Implementation(float Value)
{
	MaxHealth = Value;

	if(UpdateHealth.IsBound())
		UpdateHealth.Broadcast();
}

void UHealthComponent::SetFullHealth_Implementation()
{
	SetHealth(MaxHealth);

	if(UpdateHealth.IsBound())
		UpdateHealth.Broadcast();
}

void UHealthComponent::Initialized(float CurrentValue, float MaxValue)
{
	CurrentHealth = CurrentValue;
	MaxHealth = MaxValue;
	IsInitialized = true;
	
	if(UpdateHealth.IsBound())
		UpdateHealth.Broadcast();
}

float UHealthComponent::GetHealthInPercent() const
{
	if(!IsInitialized)
		return 0;
	
	const float health = static_cast<float>(CurrentHealth) / static_cast<float>(MaxHealth);
	
	return health;
}

