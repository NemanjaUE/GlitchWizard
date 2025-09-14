// Fill out your copyright notice in the Description page of Project Settings.


#include "Spells/TPose.h"

void ATPose::PerformSpell()
{
	if (bIsSpellOnCooldown) { return; }

	PlayerPawn->bIsTPoseSpellActive = true;

	bIsSpellOnCooldown = true;

	StartCooldownResetTimer();

	GetWorldTimerManager().SetTimer(TPoseActiveResetTimer, [this]() { PlayerPawn->bIsTPoseSpellActive = false; }, SpellLength, false);
}

void ATPose::StartCooldownResetTimer()
{
	Super::StartCooldownResetTimer();
}
