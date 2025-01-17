// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAnimNotify.h"

#include "HumanBase.h"
#include "HumanoidAnim.h"

void UAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(GetWorld()->GetNetMode() == NM_Client)
		return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("Custom AnimNotify triggered!"));
	}

	if(const UHumanoidAnim* HumanAnim = Cast<UHumanoidAnim>(MeshComp->GetAnimInstance()))
		HumanAnim->HumanBase->Attack();
	
}
