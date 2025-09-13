// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class GLITCHWIZ_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
public:
		ABaseAIController();
protected:
	virtual void BeginPlay() override;
	
};

