// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameStateBase.h"
#include "Components/BoxComponent.h"
#include "ActionCollision.generated.h"

DECLARE_DELEGATE_OneParam(FUpdateAction, EAction)

UCLASS()
class SURGAME_API UActionCollision : public UBoxComponent
{
	GENERATED_BODY()

public:
	FUpdateAction UpdateAction;

	virtual void BeginPlay() override;
	
private:
	EAction CurrentAction = EAction::None;
	
	void CheckAction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	
};
