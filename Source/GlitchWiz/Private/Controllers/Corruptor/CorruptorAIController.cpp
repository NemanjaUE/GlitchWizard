// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/Corruptor/CorruptorAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/Corruptor/Corruptor.h"

ACorruptorAIController::ACorruptorAIController()
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
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;;
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 800.0f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->ConfigureSense(*HearingConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACorruptorAIController::OnPerceptionUpdated);
}



void ACorruptorAIController::BeginPlay()
{
	Super::BeginPlay();
	PerceptionComponent->RequestStimuliListenerUpdate();

}

void ACorruptorAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;
	if (CurrentTarget)
	{
		float Distance = ControlledPawn->GetDistanceTo(CurrentTarget);
		bool bInSightRange = (Distance <= SightConfig->SightRadius);
		bool bInEngagementRange = (Distance <= EngagementRange);
		if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		{
			BlackboardComp->SetValueAsBool(TEXT("InSightRange"), bInSightRange);
			BlackboardComp->SetValueAsBool(TEXT("InEngagementRange"), bInEngagementRange);
			BlackboardComp->SetValueAsObject(TEXT("CurrentTarget"), CurrentTarget);
			BlackboardComp->SetValueAsName(TEXT("CurrentState"), CurrentState);
			
		}
	}
	else if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsBool(TEXT("InSightRange"), false);
		BlackboardComp->SetValueAsBool(TEXT("InEngagementRange"), false);
		BlackboardComp->SetValueAsObject(TEXT("CurrentTarget"), nullptr);
	}
}



void ACorruptorAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
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
void ACorruptorAIController::SetState(FName NewState)
{
	CurrentState = NewState;
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsName(TEXT("CurrentState"), NewState);
	}
	if (ACorruptor* Corruptor = Cast<ACorruptor>(GetPawn()))
	{
		if (NewState == TEXT("Idle"))
			Corruptor->CorruptorState = ECorruptorState::Idle;
		else if (NewState == TEXT("Chase"))
			Corruptor->CorruptorState = ECorruptorState::Chase;
		else if (NewState == TEXT("Attack"))
			Corruptor->CorruptorState = ECorruptorState::Attack;
	}
}
void ACorruptorAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (!InPawn) return;
	if (ACorruptor* ControlledPawn = Cast<ACorruptor>(InPawn))
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
