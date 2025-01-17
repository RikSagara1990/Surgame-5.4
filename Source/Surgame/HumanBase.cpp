// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanBase.h"

#include "HumanoidAnim.h"
#include "UnitDataBase.h"
#include "UnitWidget.h"
#include "VectorTypes.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AHumanBase::AHumanBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	bReplicates = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	AttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Collision"));
	AttackCollision->SetBoxExtent(FVector(140, 140, 90));
	AttackCollision->bDynamicObstacle = true;
	AttackCollision->SetupAttachment(GetRootComponent());
	const FVector initialPosition = FVector(0, 0, 0);
	FTransform initialTransform = FTransform();
	initialTransform.SetTranslation(initialPosition);
	AttackCollision->SetRelativeTransform(initialTransform);
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetOnlyOwnerSee(false);
	Widget->SetOwnerNoSee(true);
	Widget->SetupAttachment(GetRootComponent());
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

// Called when the game starts or when spawned
void AHumanBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsInitialized)
		Initialize();
}

// Called every frame
void AHumanBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// if(GetWorld()->GetNetMode() != NM_Client)
	// {
	// 	UpdateCurrentSpeed(DeltaTime);
	// }
	UpdateCurrentSpeed(DeltaTime);
	
	UpdateEndurance(DeltaTime);
}

void AHumanBase::UpdateCurrentSpeed(float DeltaTime)
{
	float TargetSpeed;
	if(ReplicationSprint && CanUseAction(EAction::Spring))
	{
		TargetSpeed = StaticData->RunSpeed;
	}
	else
	{
		ReplicationSprint = false;
		TargetSpeed = StaticData->WalkSpeed;
	}
	const float currentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	if(round(currentSpeed) == round(TargetSpeed))
		return;

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("TargetSpeed : x: %f"), TargetSpeed));
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, FString::Printf(TEXT("currentSpeed : x: %f"), currentSpeed));	
	
	const int DifferentSpeed = StaticData->RunSpeed - StaticData->WalkSpeed;
	const float AddSpeed = DifferentSpeed * (DeltaTime / fTimeChangeSpeed);
	const float NeedSpeed = FMath::Abs(TargetSpeed - currentSpeed);
	const float StepSpeed = FMath::Min(1, AddSpeed / NeedSpeed);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("StepSpeed : x: %f"), StepSpeed));

	const float speed = FMath::Lerp(currentSpeed, TargetSpeed, StepSpeed);
	GetCharacterMovement()->MaxWalkSpeed = round(speed);
}

void AHumanBase::UpdateEndurance(float DeltaTime)
{
	if(!MyGameStateBase)
		return;
		
	if(ReplicationSprint)
	{
		const float EnduranceCost = MyGameStateBase->ActionEnduranceCost[EAction::Spring] * DeltaTime;
		SetEndurance(GetEndurance() - EnduranceCost);
		
		EnduranceRecoveryDelay = MyGameStateBase->EnduranceRecoveryDelay;
	}
	else
	{
		if(EnduranceRecoveryDelay > 0)
			EnduranceRecoveryDelay -= DeltaTime;
		else
			SetEndurance(GetEndurance() + 1 * DeltaTime);
	}
}

void AHumanBase::ReplicationSetAction_Implementation(EAction Action)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Use Action : %s"), *UEnum::GetValueAsString(Action)));
	switch (Action)
	{
	case EAction::Spring:
		SetSprint(true);
		break;
	case EAction::Walk:
		SetSprint(false);
		break;
	case EAction::Jump:
		Jump();
		break;
	case EAction::Action:
		AHumanBase::Action(Action);
		break;
	default: ;
	}
}

void AHumanBase::ServerValidateAndSetAction_Implementation(const EAction Action)
{
	if(!StaticData)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Missing StaticData by HumanBase : %s"), *GetActorNameOrLabel()));
		//UE_LOG(LogTemp, Error, TEXT(UKismetSystemLibrary::GetDisplayName(this)));
		UE_LOG(LogTemp, Error, TEXT("Missing StaticData"));
		return;
	}

	if(!CanUseAction(Action))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Can't use Action : %s"), *UEnum::GetValueAsString(Action)));
		return;
	}
	
	ReplicationSetAction(Action);
}

void AHumanBase::Move(float VerticalValue, float HorizontalValue)
{
	bool IsGamepad = false;
	if(IsGamepad)
	{
		const float WorldDeltaSeconds = GetWorld()->GetTimeSeconds();
		const float TurnRateGamePad = MyGameStateBase? MyGameStateBase->TurnRateGamepad : 50;
		const float AddHorizontalValue = HorizontalValue * TurnRateGamePad * WorldDeltaSeconds;
		AddControllerYawInput(AddHorizontalValue);
		const float AddVerticalValue = VerticalValue * TurnRateGamePad * WorldDeltaSeconds;
		AddControllerPitchInput(AddVerticalValue);
	}
	else
	{
		const FRotator Rotator = GetControlRotation();
		if(VerticalValue != 0)
		{
			const FRotator* RotatorYaw = new FRotator(0, Rotator.Yaw, 0);
			const FVector WorldDirection = UKismetMathLibrary::GetForwardVector(*RotatorYaw);
			AddMovementInput(WorldDirection, VerticalValue);
			
		}
		else if(HorizontalValue != 0)
		{
			const FRotator* RotatorRoll = new FRotator(0, Rotator.Yaw, 0);
			const FVector WorldDirection = UKismetMathLibrary::GetRightVector(*RotatorRoll);
			AddMovementInput(WorldDirection, HorizontalValue);
		}
	}
}

void AHumanBase::LookAt(float VerticalValue, float HorizontalValue)
{
	AddControllerYawInput(HorizontalValue);
	AddControllerPitchInput(VerticalValue);
}

void AHumanBase::SetSprint(bool Value)
{
	if(!StaticData)
		return;

	if(ReplicationSprint == Value)
		return;

	if(Value && !CanUseAction(EAction::Spring))
		return;
	
	ReplicationSprint = Value;
}


void AHumanBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AHumanBase, ReplicationSprint);
}

#pragma region Interaction
EAction AHumanBase::GetTypeInteract()
{
	int health = HealthComponent->GetHealth();
	if(HealthComponent && HealthComponent->GetHealth() > 0)
		return EAction::Attack;
	
	return EAction::None;
}


void AHumanBase::Interact(EAction Action, const AHumanBase* HumanBase)
{
	if(Action == EAction::Attack)
		TakeHit(HumanBase);
}

FVector AHumanBase::GetPosition()
{
	return GetActorLocation();
}

#pragma endregion Interaction

void AHumanBase::TakeHit_Implementation(const AHumanBase* HumanBase)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, FString::Printf(TEXT("TakeHit")));
	if(HealthComponent)
	{
		float currentHealth = HealthComponent->GetHealth();
		currentHealth = currentHealth - 10;
		HealthComponent->SetHealth(currentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("currentHealth %f"), currentHealth));
		if(currentHealth > 0)
			HumanoidAnim->PlayActionAnim(EAction::Hit);
		else
			Dead();
	}
}

void AHumanBase::Dead() const
{
	GetCharacterMovement()->DisableMovement();
	HumanoidAnim->PlayActionAnim(EAction::Dead);
}

void AHumanBase::Jump()
{
	if(!MyGameStateBase || !HumanoidAnim || !HumanoidAnim->IsHaveAnimation(EAction::Jump))
		return;
	
	SetEndurance(GetEndurance() - MyGameStateBase->ActionEnduranceCost[EAction::Jump]);
	
	Super::Jump();

	if(HumanoidAnim)
		HumanoidAnim->PlayActionAnim(EAction::Jump);
}

void AHumanBase::Action(EAction Action)
{
	if(CurrentInteraction != nullptr)
		Action = CurrentInteraction->GetTypeInteract();
	else
		Action = GetTypeInteract();
	
	//todo end work here
	if(!HumanoidAnim->IsHaveAnimation(Action))
		return;
	
	SetEndurance(GetEndurance() - MyGameStateBase->ActionEnduranceCost[Action]);

	HumanoidAnim->PlayActionAnim(Action);

	//DrawLineTraces()
}

void AHumanBase::Attack() const
{
	for (IInteraction* interaction : InteractObjects)
	{
		if(interaction->GetTypeInteract() == EAction::Attack)
		{
			if(!CanAttack(interaction))
				continue;
			interaction->Interact(EAction::Attack, this);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, FString::Printf(TEXT("Attack")));
		}
	}
}

bool AHumanBase::CanAttack(IInteraction* Interact) const
{
	FVector ForwardVector = this->GetActorForwardVector();
	FVector InteractVector = Interact->GetPosition() - this->GetActorLocation();
	InteractVector.Normalize();
	ForwardVector.Normalize();
	const float dotProduct = FVector::DotProduct(ForwardVector, InteractVector);
	return dotProduct > 0.7;
}

void AHumanBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if(OtherActor == this)
		return;
	
	if(IInteraction* InteractionObject = Cast<IInteraction>(OtherActor))
	{
		if(!InteractObjects.Contains(InteractionObject))
		{
			InteractObjects.Add(InteractionObject);
			// const FString string = GetName() + "/" + *OtherActor->GetName();
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Added Interaction : %s"), *string));
			UpdateCurrentInteraction();
		}
	}
}

void AHumanBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor == this)
		return;
	
	if(const IInteraction* InteractionObject = Cast<IInteraction>(OtherActor))
	{
		if(InteractObjects.Contains(InteractionObject))
		{
			InteractObjects.Remove(InteractionObject);
			// const FString string = GetName() + "/" + *OtherActor->GetName();
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Ended Interaction : %s"), *string));
			UpdateCurrentInteraction();
		}
	}
}

void AHumanBase::UpdateCurrentInteraction()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("InteractObjects.Num : x: %d"), InteractObjects.Num()));
	
	for (IInteraction* InteractObject : InteractObjects)
	{
		const EAction TypeAction = InteractObject->GetTypeInteract();

		if(TypeAction == EAction::None)
			continue;
	
		if(!CurrentInteraction || CurrentInteraction->GetTypeInteract() < TypeAction)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Set new Action : %s"), *UEnum::GetValueAsString(TypeAction)));
			CurrentInteraction = InteractObject;
		}
	}
	//UpdateActionIcon
}

void AHumanBase::Initialize()
{
	MyGameStateBase = Cast<AMyGameStateBase>(UGameplayStatics::GetGameState(this));

	if(!MyGameStateBase)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Missing my MyGameStateBase")));
	
	if (!StaticData)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Missing my StaticData")));
		return;
	}
	
	MaxEndurance = StaticData->Endurance;
	SetEndurance(StaticData->Endurance);

	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AHumanBase::OnOverlapBegin);
	AttackCollision->OnComponentEndOverlap.AddDynamic(this,&AHumanBase::OnOverlapEnd);
	
	IsInitialized = true;
	if(UUnitWidget* UnitWidget = Cast<UUnitWidget>(Widget->GetUserWidgetObject()))
		UnitWidget->SetHumanBase(this);
	
	if(HealthComponent)
	{
		int StartHealth = StaticData->Health;
		HealthComponent->Initialized(StartHealth, StartHealth);
	}
}

void AHumanBase::AddCharacterLevel()
{
	CurrentLevel++;
	SetCharacterLevel(GetCharacterLevel() + 1);
	UpdateCurrentUnitParameters.ExecuteIfBound();
}

bool AHumanBase::CanUseAction(const EAction Action) const
{
	if(!MyGameStateBase)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Missing MyGameStateBase!")));
		return false;
	}

	if(!HumanoidAnim)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Missing HumanoidAnim!")));
		return false;
	}

	if(Action == EAction::Jump)
	{
		if(bPressedJump || HumanoidAnim->isInAir || HumanoidAnim->IsPlayAnimation(Bottom))
			return false;
	}
	else if(MyGameStateBase->IsActionHands(Action))
	{
		if(HumanoidAnim->IsPlayAnimation(Upper))
			return false;
		
		if(!CurrentInteraction)
			return IsHaveEnduranceForAction(EAction::Attack);
		
		if(CurrentInteraction->GetTypeInteract() != Action)
			return IsHaveEnduranceForAction(CurrentInteraction->GetTypeInteract());
	}

	if(!IsHaveEnduranceForAction(Action))
		return false;
	
	return true;
}

bool AHumanBase::IsHaveEnduranceForAction(EAction Action) const
{
	if (MyGameStateBase->ActionEnduranceCost.Contains(Action))
		if (GetEndurance() < MyGameStateBase->ActionEnduranceCost[Action])
			return false;

	return true;
}


