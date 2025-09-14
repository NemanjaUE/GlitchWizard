// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTasks/AttackTaskNode.h"
#include "Controllers/Corruptor/CorruptorAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/Corruptor/Corruptor.h"
#include "Controllers/Corruptor/CorruptorAIController.h"
#include "Animation/AnimInstance.h"
UAttackTaskNode::UAttackTaskNode()
{
	NodeName = TEXT("AttackTask");
	bNotifyTick = true;
	
}

EBTNodeResult::Type UAttackTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;
	ACorruptorAIController* AIController = Cast<ACorruptorAIController>(Controller);
	if (!AIController) return EBTNodeResult::Failed;
	AActor* TargetActor = AIController->CurrentTarget;
	
	Corruptor = Cast<ACorruptor>(Controller->GetPawn());
	float Distance = Corruptor->GetDistanceTo(TargetActor);
	float MinAttackDistance = 100.f;
	float MaxAttackDistance = 150.f;
	if (Distance < MinAttackDistance || Distance > MaxAttackDistance)
	{
		return EBTNodeResult::Failed;
	}
	Corruptor->StartAttack();
	bWaitingForAttackToFinish = true;
	return EBTNodeResult::InProgress;
}

void UAttackTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!Corruptor && !Corruptor->GetMesh()) return;
	UAnimInstance* AnimInstance = Corruptor->GetMesh()->GetAnimInstance();
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(Corruptor->AttackMontage))
	{
		Corruptor->bIsAttacking = false;
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsAttacking", false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		
	}
}
void UAttackTaskNode::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,EBTNodeResult::Type TaskResult)
{
	bWaitingForAttackToFinish = false;
}

