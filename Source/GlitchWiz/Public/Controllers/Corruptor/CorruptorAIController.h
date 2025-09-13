// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/BaseAIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "CorruptorAIController.generated.h"

/**
 * 
 */
UCLASS()
class GLITCHWIZ_API ACorruptorAIController : public ABaseAIController
{
	GENERATED_BODY()
public:
	ACorruptorAIController();
	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;
	UPROPERTY()
	UAISenseConfig_Hearing* HearingConfig;
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor , FAIStimulus Stimulus);
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	AActor* CurrentTarget;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BlackboardComponent;
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetState(FName NewState);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName CurrentState = TEXT("Idle");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float EngagementRange = 300.0f;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
};
