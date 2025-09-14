// Fill out your copyright notice in the Description page of Project Settings.


#include "Spells/Iceball.h"

void AIceball::PerformSpell()
{
	if (bIsSpellOnCooldown) { return; }

	bIsSpellOnCooldown = true;

	UE_LOG(LogTemp, Warning, TEXT("Perform iceball spell"));

	AIceballProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AIceballProjectile>(
						ProjectileClass,
						PlayerPawn->GetActorLocation(),
						PlayerPawn->GetActorRotation()
	);

	SpawnedProjectile->FindComponentByClass<UStaticMeshComponent>()->AddImpulse(CameraComp->GetForwardVector() * FVector(100, 100, 100));

	StartCooldownResetTimer();
}

void AIceball::StartCooldownResetTimer()
{
	Super::StartCooldownResetTimer();
}
