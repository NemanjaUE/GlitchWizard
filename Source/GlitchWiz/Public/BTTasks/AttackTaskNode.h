// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AttackTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class GLITCHWIZ_API UAttackTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UAttackTaskNode();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UPROPERTY()
	class ACorruptor* Corruptor;
	bool bWaitingForAttackToFinish = false;
};
