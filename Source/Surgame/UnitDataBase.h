// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UnitDataBase.generated.h"

/**
 * 
 */
UCLASS()
class SURGAME_API UUnitDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Endurance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level;
};
