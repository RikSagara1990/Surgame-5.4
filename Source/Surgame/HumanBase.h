// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "Interaction.h"
#include "MyGameStateBase.h"
#include "GameFramework/Character.h"
#include "HumanBase.generated.h"

DECLARE_DELEGATE(FUpdateCurrentUnitParameters)
DECLARE_DELEGATE(FUpdateCurrentUnitEndurance)
DECLARE_DELEGATE(FUpdateCurrentUnitExperience)

UCLASS()
class SURGAME_API AHumanBase : public ACharacter, public IInteraction
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHumanBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HumanBase")
	bool IsInitialized;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "HumanBase")
	bool ReplicationSprint = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HumanBase")
	float fTimeChangeSpeed = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HumanBase")
	class UHumanoidAnim* HumanoidAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HumanBase")
	class UUnitDataBase* StaticData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Control")
	class AMyPlayerController* MyPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	class AMyPlayerController* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HumanBase")
	class UBoxComponent* AttackCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HumanBase")
	class UWidgetComponent* Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HumanBase")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HumanBase")
	UEnduranceComponent* EnduranceComponent;
	
	// float GetEndurance() const { return CurrentEndurance; }
	// void SetEndurance(float val) {
	// 	CurrentEndurance = val; 
	// 	if (CurrentEndurance < 0)
	// 		CurrentEndurance = 0;
	// 	if (CurrentEndurance > MaxEndurance)
	// 		CurrentEndurance = MaxEndurance;
	// 	UpdateCurrentUnitEndurance.ExecuteIfBound();
	// }

	int GetCharacterLevel() const { return CurrentLevel; }
	void SetCharacterLevel(int val) {
		if (CurrentLevel < 1) {
			for (int i = CurrentLevel; i <= val ; i++)
				AddCharacterLevel();
		}	
	}

	bool IsDead() const
	{
		if(!HealthComponent)
			return true;
		
		return HealthComponent->GetHealth() < 1;
	} 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Move(float VerticalValue, float HorizontalValue);
	void LookAt(float VerticalValue, float HorizontalValue);
	UFUNCTION(BlueprintCallable)
	void SetSprint(bool Value);
	virtual void Jump() override;
	void Action(EAction Action);
	void Attack() const;
	
	UFUNCTION(Reliable, Server)
	void ServerValidateAndSetAction(EAction Action);
	UFUNCTION(Reliable, NetMulticast)
	void ReplicationSetAction(EAction Action);
	FUpdateCurrentUnitParameters UpdateCurrentUnitParameters;
	FUpdateCurrentUnitEndurance UpdateCurrentUnitEndurance;
	FUpdateCurrentUnitExperience UpdateCurrentUnitExperience;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma region Interaction
	virtual EAction GetTypeInteract() override;
	virtual void Interact(EAction Action, const AHumanBase* HumanBase) override;
	virtual FVector GetPosition() override;
#pragma endregion Interaction
	

private:
	float EnduranceRecoveryDelay = 0;
	EAction CurrentHandAction = EAction::None;
	TSet<IInteraction*> InteractObjects;
	IInteraction* CurrentInteraction;
	float CurrentEndurance;
	int MaxEndurance;
	int CurrentLevel;
	
	UPROPERTY()
	class AMyGameStateBase* MyGameStateBase;
	
	void Initialize();
	void AddCharacterLevel();
	bool CanUseAction(EAction Action) const;
	bool IsHaveEnduranceForAction(EAction Action) const;
	void UpdateCurrentSpeed(float DeltaTime);
	void UpdateEndurance(float DeltaTime);
	UFUNCTION(Reliable, NetMulticast)
	void TakeHit(const AHumanBase* HumanBase);
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void UpdateCurrentInteraction();
	bool CanAttack(IInteraction* Interact) const;
	void Dead() const;
	
	// UFUNCTION(Reliable, Server)
	// void ReplicationSetSprint(bool Value);
};
