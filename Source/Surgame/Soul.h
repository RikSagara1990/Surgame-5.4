// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HumanBase.h"
#include "Soul.generated.h"

UCLASS()
class SURGAME_API ASoul : public AHumanBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASoul();

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* Camera;
};
