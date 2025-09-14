#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ManipulatorAttackNode.generated.h"

UCLASS()
class GLITCHWIZ_API UManipulatorAttackNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UManipulatorAttackNode();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	UPROPERTY()
	class AManipulatorCharacter* Manipulator;

	bool bWaitingForAttackToFinish = false;
};
