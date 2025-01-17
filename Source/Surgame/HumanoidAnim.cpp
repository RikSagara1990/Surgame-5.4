// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanoidAnim.h"

#include "KismetAnimationLibrary.h"
#include "GameFramework/PawnMovementComponent.h"

void UHumanoidAnim::NativeBeginPlay()
{
    Super::NativeBeginPlay();
    Initialize();
}

void UHumanoidAnim::Initialize()
{
    HumanBase = Cast<AHumanBase>(TryGetPawnOwner());
    if(HumanBase)
        HumanBase->HumanoidAnim = this;
}

void UHumanoidAnim::FinishedAnimation(EActionType AnimType)
{
    if(HumanBase->IsDead())
        return;
    
    if(AnimType == Upper)
    {
        IsPlayUpperAnimation = false;
        GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, FString::Printf(TEXT("Finish Upper Animation")));
    }
    else if(AnimType == Bottom)
        IsPlayBottomAnimation = false;
}

void UHumanoidAnim::StartAnimation(const EActionType AnimType, UAnimMontage* AnimMontage)
{
    const float AnimDuration = Montage_Play(AnimMontage);
    if(AnimDuration > 0)
    {
        FTimerHandle TimerHandle;
        if (AnimType == Full)
        {
            IsPlayBottomAnimation = true;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){FinishedAnimation(Full);}, AnimDuration, false);
        }
        else if (AnimType == Upper)
        {
            IsPlayUpperAnimation = true;
            GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Blue, FString::Printf(TEXT("Start Upper Animation")));
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){FinishedAnimation(Upper);}, AnimDuration, false);
        }
    }
}

void UHumanoidAnim::DeadAnimation()
{
    if(ActionAnimMontages.Contains(Action))
    {
        Montage_Stop(0);
        Montage_Play(ActionAnimMontages[Action]);
        IsPlayBottomAnimation = true;
        IsPlayUpperAnimation = true;
    }
}

bool UHumanoidAnim::IsPlayAnimation(EActionType AnimType) const
{
    if(AnimType == Any)
        return IsPlayBottomAnimation || IsPlayUpperAnimation;
    if(AnimType == Bottom)
        return IsPlayBottomAnimation;
    if(AnimType == Upper)
        return IsPlayUpperAnimation;
    
    return false;
}

EActionType UHumanoidAnim::GetActionType(EAction Action)
{
    switch (Action)
    {
    case EAction::Spring:
        return Bottom;
    case EAction::Walk:
        return Bottom;
    case EAction::Jump:
        return Bottom;
    case EAction::Action:
        return Full;
    case EAction::Attack:
        return Upper;
    case EAction::Dead:
        return Full;
    case EAction::Hit:
        return Full;
    default: return Any;
    }
}

void UHumanoidAnim::NativeUpdateAnimation(float DeltaTimeX)
{
    //Very Important Line
    Super::NativeUpdateAnimation(DeltaTimeX);

    //Always Check Pointers
    if (!HumanBase)
    {
        return;
    }

    isInAir = HumanBase->GetMovementComponent()->IsFalling();
    
    if(isInAir)
    {
        Speed = 0;
        Direction = 0;
        return;
    }

    Speed = HumanBase->GetVelocity().Size();
    const FVector Velocity = HumanBase->GetVelocity();
    const FRotator Rotation = HumanBase->GetActorRotation();
    Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Rotation);
}

void UHumanoidAnim::PlayActionAnim(EAction Action)
{
    if(Action == EAction::Dead)
        DeadAnimation();
        
    if(ActionAnimMontages.Contains(Action))
    {
        const EActionType ActionType = GetActionType(Action);
        StartAnimation(ActionType, ActionAnimMontages[Action]);
    }
}

bool UHumanoidAnim::IsHaveAnimation(EAction Action) const
{
    return ActionAnimMontages.Contains(Action);
}




