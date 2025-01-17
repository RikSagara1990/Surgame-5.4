// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"

#include "HumanBase.h"
#include "Blueprint/UserWidget.h"

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void AMyHUD::Initialize()
{
	if(PlayerWidgetClass != nullptr)
	{
		CurrentPlayerWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerWidgetClass);
		if(CurrentPlayerWidget)
			CurrentPlayerWidget->AddToViewport();
	}
	
}
