// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FUpdateHealth)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	
	UFUNCTION(Reliable, NetMulticast)
	void SetHealth(float Value);
	UFUNCTION(Reliable, NetMulticast)
	void SetMaxHealth(float Value);
	UFUNCTION(Reliable, NetMulticast)
	void SetFullHealth();

	void Initialized(float CurrentValue, float MaxValue);
	float GetHealthInPercent() const;
	float GetHealth() const { return CurrentHealth; }

	FUpdateHealth UpdateHealth;
	
private:
	int CurrentHealth = 0;
	int MaxHealth = 0;
	bool IsInitialized = false;
};
