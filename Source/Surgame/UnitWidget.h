// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HumanBase.h"
#include "Blueprint/UserWidget.h"
#include "UnitWidget.generated.h"

/**
 * 
 */
UCLASS()
class SURGAME_API UUnitWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHumanBase(AHumanBase* InHumanBase);
	void Update_HealthBar() const;

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NameBlock;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthProgressBar;

private:
	AHumanBase* HumanBase;
	UHealthComponent* HealthComponent;
};
