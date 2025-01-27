// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HumanBase.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class SURGAME_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHumanBase(AHumanBase* InHumanBase);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AHumanBase* HumanBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UEnduranceComponent* EnduranceComponent;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* EnduranceProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ExperienceProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LevelTextBlock;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;

private:
	void Update_UnitParameters() const;
	void Update_HealthBar() const;
	void Update_EnduranceBar() const;
	void Update_ExperienceBar() const;
	void Update_Level() const;
};
