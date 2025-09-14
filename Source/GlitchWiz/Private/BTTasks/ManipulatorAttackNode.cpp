#include "BTTasks/ManipulatorAttackNode.h"
#include "Controllers/Manipulator/ManipulatorAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/Manipulator/ManipulatorCharacter.h"
#include "Animation/AnimInstance.h"

UManipulatorAttackNode::UManipulatorAttackNode()
{
	NodeName = TEXT("Manipulator Attack Task");
	bNotifyTick = true;
}

EBTNodeResult::Type UManipulatorAttackNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	AManipulatorAIController* AIController = Cast<AManipulatorAIController>(Controller);
	if (!AIController) return EBTNodeResult::Failed;

	AActor* TargetActor = AIController->CurrentTarget;
	Manipulator = Cast<AManipulatorCharacter>(Controller->GetPawn());
	if (!Manipulator || !TargetActor) return EBTNodeResult::Failed;

	float Distance = Manipulator->GetDistanceTo(TargetActor);
	float MinAttackDistance = 100.f;
	float MaxAttackDistance = 700.f;

	
	if (!Manipulator->bIsAttacking)
	{
		if (Distance < MinAttackDistance || Distance > MaxAttackDistance)
		{
			return EBTNodeResult::Failed;
		}

		Manipulator->StartAttack();
		Manipulator->bIsAttacking = true;
		bWaitingForAttackToFinish = true;

		
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsAttacking", true);
	}

	return EBTNodeResult::InProgress;
}

void UManipulatorAttackNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!Manipulator || !Manipulator->GetMesh()) return;

	UAnimInstance* AnimInstance = Manipulator->GetMesh()->GetAnimInstance();
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(Manipulator->AttackMontage))
	{
		
		Manipulator->bIsAttacking = false;
		bWaitingForAttackToFinish = false;

		
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsAttacking", false);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UManipulatorAttackNode::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	bWaitingForAttackToFinish = false;
}
