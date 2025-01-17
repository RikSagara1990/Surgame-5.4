// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitWidget.h"
#include <Components/ProgressBar.h>

#include "Components/TextBlock.h"
#include "PlayerWidget.h"

void UUnitWidget::SetHumanBase(AHumanBase* InHumanBase)
{
	HumanBase = InHumanBase;
	NameBlock->SetText(FText::FromString(HumanBase->GetName()));
	Update_HealthBar();
	HealthComponent = HumanBase->HealthComponent;
	 if(HealthComponent)
	 	HealthComponent->UpdateHealth.AddUObject(this, &UUnitWidget::Update_HealthBar);
}

void UUnitWidget::Update_HealthBar() const
{
	if(!HealthComponent)
		return;
	
	HealthProgressBar->SetPercent(HealthComponent->GetHealthInPercent());
}
