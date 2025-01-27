// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerWidget.h"
#include <Components/ProgressBar.h>

#include "EnduranceComponent.h"
#include "Components/TextBlock.h"

void UPlayerWidget::SetHumanBase(AHumanBase* InHumanBase)
{ HumanBase = InHumanBase; }

void UPlayerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HumanBase = Cast<AHumanBase>(GetOwningPlayerPawn());
	if(HumanBase)
	{
		HumanBase->UpdateCurrentUnitParameters.BindUObject(this, &UPlayerWidget::Update_UnitParameters);
		HumanBase->UpdateCurrentUnitExperience.BindUObject(this, &UPlayerWidget::Update_ExperienceBar);
		
		HealthComponent = HumanBase->HealthComponent;
		if(!HealthComponent)
			HealthComponent = HumanBase->FindComponentByClass<UHealthComponent>();
		if(HealthComponent)
			HealthComponent->UpdateHealth.AddUObject(this, &UPlayerWidget::Update_HealthBar);
		Update_HealthBar();

		EnduranceComponent = HumanBase->EnduranceComponent;
		if(!EnduranceComponent)
			EnduranceComponent = HumanBase->FindComponentByClass<UEnduranceComponent>();
		if(EnduranceComponent)
			EnduranceComponent->UpdateEndurance.AddUObject(this, &UPlayerWidget::Update_EnduranceBar);
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("HumanBase is not found")));	
}

void UPlayerWidget::Update_UnitParameters() const
{
	Update_HealthBar();
	Update_EnduranceBar();
	Update_ExperienceBar();
	Update_Level();
}

void UPlayerWidget::Update_HealthBar() const
{
	if(!HealthComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("HealthProgressBar is not missing")));	
		HealthProgressBar->SetPercent(0);
		return;
	}
	HealthProgressBar->SetPercent(HealthComponent->GetHealthInPercent());
}

void UPlayerWidget::Update_EnduranceBar() const
{
	if(!EnduranceComponent)
		return;
	
	EnduranceProgressBar->SetPercent(EnduranceComponent->GetEnduranceInPercent());
}

void UPlayerWidget::Update_ExperienceBar() const
{
	ExperienceProgressBar->SetPercent(0);
}

void UPlayerWidget::Update_Level() const
{
	const FString CurrenLevel= FString::FromInt(HumanBase->GetCharacterLevel());
	LevelTextBlock->SetText(FText::FromString(CurrenLevel));
}
