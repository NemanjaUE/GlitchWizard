// Fill out your copyright notice in the Description page of Project Settings.


#include "Spells/TextureMagic.h"

void ATextureMagic::PerformSpell()
{
	if (bIsSpellOnCooldown) { return; }

	bIsTextureMagicSpellActive = true;

	bIsSpellOnCooldown = true;

	StartCooldownResetTimer();

	GetWorldTimerManager().SetTimer(TextureMagicActiveResetTimer, [this]() { bIsTextureMagicSpellActive = false; }, SpellLength, false);
}

void ATextureMagic::StartCooldownResetTimer()
{
	Super::StartCooldownResetTimer();
}
