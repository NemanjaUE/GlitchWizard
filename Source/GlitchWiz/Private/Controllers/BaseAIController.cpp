// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Controllers/BaseAIController.h"

ABaseAIController::ABaseAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	PerceptionComponent->bAutoRegister = true;
}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();
}
