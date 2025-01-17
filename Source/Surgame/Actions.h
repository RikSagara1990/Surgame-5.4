// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actions.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EAction
{
	None UMETA(DisplayName = "None"),
	Spring UMETA(DisplayName = "Spring"),
	Walk UMETA(DisplayName = "Walk"),
	Jump UMETA(DisplayName = "Jump"),
	Action UMETA(DisplayName = "Action"),
	Attack UMETA(DisplayName = "Attack"),
	Dead UMETA(DisplayName = "Dead"),
	Hit UMETA(DisplayName = "Hit")
};

enum EActionType
{
	Upper, Bottom, Full, Any
};