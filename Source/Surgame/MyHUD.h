// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class SURGAME_API AMyHUD : public AHUD
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category= "UserWidget")
	TSubclassOf<UUserWidget> PlayerWidgetClass;

	UPROPERTY(EditAnywhere, Category= "UserWidget")
	class UUserWidget* CurrentPlayerWidget;

public:
	void Initialize();

	UPROPERTY(EditAnywhere, Category= "UserData")
	class AHumanBase* HumanBase;

protected:
	virtual void BeginPlay() override;
	
};
