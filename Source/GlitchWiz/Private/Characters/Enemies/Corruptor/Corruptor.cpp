


#include "Characters/Enemies/Corruptor/Corruptor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controllers/Corruptor/CorruptorAIController.h"


ACorruptor::ACorruptor()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_CorruptorObj(TEXT("/Game/AI/Corruptor/BT_Corruptor.BT_Corruptor"));
	if (BT_CorruptorObj.Succeeded())
	{
		CustomBehaviorTree = BT_CorruptorObj.Object;
	}
}
void ACorruptor::BeginPlay()
{
	Super::BeginPlay();
	
}


void ACorruptor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ACorruptorAIController* CorruptorAiController = Cast<ACorruptorAIController>(GetController());
	if (!CorruptorAiController) return;
	UBlackboardComponent* Blackboard = CorruptorAiController->GetBlackboardComponent();
	if (!Blackboard) return;
	AActor* Target = CorruptorAiController->CurrentTarget;
	if (Target)
	{
		float Distance = GetDistanceTo(Target);
		bool bInAttackRange = Distance <= AttackRange;
		if (bInAttackRange)
		{
			CorruptorAiController->SetState("Attack");
		}
		else
		{
			CorruptorAiController->SetState("Chase");
		}
		
	}
	else
	{
		CorruptorAiController->SetState("Idle");
	}
}

UBehaviorTree* ACorruptor::GetBehaviorTree() const
{
	return CustomBehaviorTree;
}


float ACorruptor::GetMovementSpeed() const
{
	if (GetCharacterMovement())
	{
		return GetCharacterMovement()->Velocity.Size();
	}
	return 0.0f;
}

void ACorruptor::StartAttack()
{
	ACorruptorAIController* CorruptorAiController = Cast<ACorruptorAIController>(GetController());
	if (!CorruptorAiController) return;
	AActor* Target = CorruptorAiController->CurrentTarget;
	if (Target && GetDistanceTo(Target) <= AttackRange)
	{
		bIsAttacking = true;
		if (AttackMontage && GetMesh() && GetMesh()->GetAnimInstance())
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Stop(0.1f);
			AnimInstance->Montage_Play(AttackMontage, 1.0f);
		}
	}
	else
	{
		bIsAttacking = false;
	}
}

