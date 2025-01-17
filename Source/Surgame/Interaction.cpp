// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction.h"

EAction IInteraction::GetTypeInteract()
{
	return None;
}

void IInteraction::Interact(EAction Action, const AHumanBase* HumanBase)
{
	return;
}

FVector IInteraction::GetPosition()
{
	return FVector(0,0,0);
}
