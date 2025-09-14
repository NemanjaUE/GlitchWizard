// Fill out your copyright notice in the Description page of Project Settings.


#include "Spells/TextureMagic.h"

void ATextureMagic::PerformSpell()
{
	if (bIsSpellOnCooldown) { return; }

	PlayerPawn->bIsTextureMagicSpellActive = true;

	bIsSpellOnCooldown = true;

	StartCooldownResetTimer();

	GetWorldTimerManager().SetTimer(TextureMagicActiveResetTimer, [this]() { PlayerPawn->bIsTextureMagicSpellActive = false; }, SpellLength, false);
}

void ATextureMagic::StartCooldownResetTimer()
{
	Super::StartCooldownResetTimer();
}
