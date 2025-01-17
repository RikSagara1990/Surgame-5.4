// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"

bool AMyGameStateBase::IsActionHands(EAction Action)
{
	switch (Action)
	{
	case EAction::Action:
		return true;
	default:
		return false;
	}
}
