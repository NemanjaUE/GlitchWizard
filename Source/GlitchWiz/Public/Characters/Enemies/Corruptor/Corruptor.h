// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "Corruptor.generated.h"

UENUM(BlueprintType)
enum class ECorruptorState : uint8
{
	Idle,
	Chase,
	Attack,
};
UCLASS()
class GLITCHWIZ_API ACorruptor : public ACharacter
{
	GENERATED_BODY()

public:
	
	ACorruptor();

protected:

	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;
	virtual UBehaviorTree* GetBehaviorTree() const;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Corruptor")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Corruptor")
	float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* CustomBehaviorTree;
	UFUNCTION(BlueprintCallable, Category = "Animation")
	float GetMovementSpeed() const;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Corruptor")
	ECorruptorState CorruptorState = ECorruptorState::Idle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Tombstone")
	bool bIsAttacking = false;
	void StartAttack();
	

};
