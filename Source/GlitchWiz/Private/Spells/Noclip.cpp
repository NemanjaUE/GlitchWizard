// Fill out your copyright notice in the Description page of Project Settings.


#include "Spells/Noclip.h"

#include "Components/CapsuleComponent.h"

void ANoclip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (PlayerPawn->bIsNoclipSpellActive)
	{
		TimeUntilSpellEnds -= DeltaTime;
	}
	else
	{
		TimeUntilSpellEnds = 0;
	}

	UE_LOG(LogTemp, Warning, TEXT("Time until spell ends: %f"), TimeUntilSpellEnds)
}

void ANoclip::PerformSpell()
{
	if (bIsSpellOnCooldown) { return; }

	// PlayerPawn->FindComponentByClass<UCapsuleComponent>()->SetCollisionResponseToChannel(
	// 	ECC_GameTraceChannel6, ECR_Ignore);
	// PlayerPawn->FindComponentByClass<UCapsuleComponent>()->SetCollisionResponseToChannel(
	// 	ECC_GameTraceChannel2, ECR_Ignore);

	PlayerPawn->bIsNoclipSpellActive = true;
	PlayerPawn->AmountOfActiveEffects += 1;

	TimeUntilSpellEnds = SpellLength;

	bIsSpellOnCooldown = true;

	StartCooldownResetTimer();

	GetWorldTimerManager().SetTimer(NoclipActiveResetTimer, [this]()
	{
		PlayerPawn->bIsNoclipSpellActive = false;
		PlayerPawn->AmountOfActiveEffects -= 1;
		UE_LOG(LogTemp, Warning, TEXT("Noclip not active"))
	}, SpellLength, false);
}

void ANoclip::StartCooldownResetTimer()
{
	Super::StartCooldownResetTimer();
}
