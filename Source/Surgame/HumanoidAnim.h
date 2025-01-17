// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HumanBase.h"
#include "Animation/AnimInstance.h"
#include "HumanoidAnim.generated.h"

/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class SURGAME_API UHumanoidAnim : public UAnimInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "AnimationVar")
	bool IsPlayUpperAnimation;
	bool IsPlayBottomAnimation;
	
	void Initialize();
	void FinishedAnimation(EActionType AnimType);
	void StartAnimation(EActionType AnimType, UAnimMontage* AnimMontage);
	void DeadAnimation();

protected:
	// Called when the game starts or when spawned
	virtual void NativeBeginPlay() override;

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationVar")
	class AHumanBase* HumanBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimationVar")
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimationVar")
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimationVar")
	bool isInAir;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Action Montage")
	TMap<TEnumAsByte<EAction>, UAnimMontage*> ActionAnimMontages; 

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;
	void PlayActionAnim(EAction Action);
	bool IsHaveAnimation(EAction Action) const;
	bool IsPlayAnimation(EActionType AnimType = Any) const;
	static EActionType GetActionType(EAction Action);
};
