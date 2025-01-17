// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionCollision.h"

#include "HumanBase.h"

void UActionCollision::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UActionCollision::CheckAction);
}

void UActionCollision::CheckAction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if(AHumanBase* HumanBase = Cast<AHumanBase>(OtherActor))
	{
		CurrentAction = EAction::Action;
		UpdateAction.ExecuteIfBound(CurrentAction);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Action")));
	}
}
