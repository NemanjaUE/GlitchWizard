// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/Manipulator/ManipulatorAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/Manipulator/ManipulatorCharacter.h"

AManipulatorAIController::AManipulatorAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 2500.0f;
	SightConfig->LoseSightRadius = 1000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 200.0f;
	SightConfig->SetMaxAge(5.0f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 0.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 800.0f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->ConfigureSense(*HearingConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AManipulatorAIController::OnPerceptionUpdated);
}

void AManipulatorAIController::BeginPlay()
{
	Super::BeginPlay();
	PerceptionComponent->RequestStimuliListenerUpdate();
}

void AManipulatorAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	if (CurrentTarget)
	{
		float Distance = ControlledPawn->GetDistanceTo(CurrentTarget);
		bool bInSightRange = (Distance <= SightConfig->SightRadius);

		if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		{
			BlackboardComp->SetValueAsBool(TEXT("InSightRange"), bInSightRange);
			BlackboardComp->SetValueAsObject(TEXT("CurrentTarget"), CurrentTarget);
			BlackboardComp->SetValueAsName(TEXT("CurrentState"), CurrentState);
		}
	}
	else if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsBool(TEXT("InSightRange"), false);
		BlackboardComp->SetValueAsObject(TEXT("CurrentTarget"), nullptr);
	}
}

void AManipulatorAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || Actor == GetPawn() || Actor->ActorHasTag("Enemy")) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		CurrentTarget = Actor;
		if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		{
			BlackboardComp->SetValueAsObject(TEXT("CurrentTarget"), Actor);
			if (CurrentState == TEXT("Idle"))
			{
				SetState(TEXT("Chase"));
			}
		}
	}
	else if (CurrentTarget == Actor)
	{
		CurrentTarget = nullptr;
		if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		{
			BlackboardComp->SetValueAsObject(TEXT("CurrentTarget"), nullptr);
			if (CurrentState == TEXT("Chase"))
			{
				SetState(TEXT("Idle"));
			}
		}
	}
}

void AManipulatorAIController::SetState(FName NewState)
{
	CurrentState = NewState;
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsName(TEXT("CurrentState"), NewState);
	}

	if (AManipulatorCharacter* Manipulator = Cast<AManipulatorCharacter>(GetPawn()))
	{
		if (NewState == TEXT("Idle"))
			Manipulator->ManipulatorState = EManipulatorState::Idle;
		else if (NewState == TEXT("Chase"))
			Manipulator->ManipulatorState = EManipulatorState::Chase;
		else if (NewState == TEXT("Attack"))
			Manipulator->ManipulatorState = EManipulatorState::Attack;
	}
}

void AManipulatorAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (!InPawn) return;

	if (AManipulatorCharacter* ControlledPawn = Cast<AManipulatorCharacter>(InPawn))
	{
		if (UBehaviorTree* BT = ControlledPawn->GetBehaviorTree())
		{
			BehaviorComp = BT;
			if (UseBlackboard(BT->GetBlackboardAsset(), BlackboardComponent))
			{
				RunBehaviorTree(BT);
				if (BlackboardComponent)
				{
					BlackboardComponent->SetValueAsName(TEXT("CurrentState"), TEXT("Idle"));
				}
			}
		}
	}
}
