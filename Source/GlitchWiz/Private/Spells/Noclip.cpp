// Fill out your copyright notice in the Description page of Project Settings.


#include "Spells/Noclip.h"

#include "Components/CapsuleComponent.h"

void ANoclip::PerformSpell()
{
	if (bIsSpellOnCooldown) { return; }

	PlayerPawn->FindComponentByClass<UCapsuleComponent>()->SetCollisionResponseToChannel(
		ECC_GameTraceChannel6, ECR_Ignore);
	PlayerPawn->FindComponentByClass<UCapsuleComponent>()->SetCollisionResponseToChannel(
		ECC_GameTraceChannel2, ECR_Ignore);

	PlayerPawn->bIsNoclipSpellActive = true;

	bIsSpellOnCooldown = true;

	StartCooldownResetTimer();

	GetWorldTimerManager().SetTimer(NoclipActiveResetTimer, [this]()
	{
		PlayerPawn->bIsNoclipSpellActive = false;
		PlayerPawn->FindComponentByClass<UCapsuleComponent>()->SetCollisionResponseToChannel(
		ECC_GameTraceChannel6, ECR_Block);
		PlayerPawn->FindComponentByClass<UCapsuleComponent>()->SetCollisionResponseToChannel(
		ECC_GameTraceChannel2, ECR_Block);
	}, SpellLength, false);
}

void ANoclip::StartCooldownResetTimer()
{
	Super::StartCooldownResetTimer();
}
