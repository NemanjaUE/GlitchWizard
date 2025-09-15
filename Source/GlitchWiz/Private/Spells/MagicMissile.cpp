// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Spells/MagicMissile.h"
#include "Engine/OverlapResult.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Projectiles/ProjectileBase.h"

void AMagicMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("bIsTextureMagicSpellActive = %s"), PlayerPawn->bIsTextureMagicSpellActive ? TEXT("true") : TEXT("false"));
	//UE_LOG(LogTemp, Warning, TEXT("bIsNoclipSpellActive = %s"), PlayerPawn->bIsNoclipSpellActive ? TEXT("true") : TEXT("false"));
	//UE_LOG(LogTemp, Warning, TEXT("bIsTPoseSpellActive = %s"), PlayerPawn->bIsTPoseSpellActive ? TEXT("true") : TEXT("false"));
}

void AMagicMissile::PerformSpell()
{
	if (bIsSpellOnCooldown) { return; }

	UE_LOG(LogTemp, Warning, TEXT("Perform magic missile spell"));

	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PlayerPawn);

	bool bHasHit = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		PlayerPawn->GetActorLocation(),
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel2),
		FCollisionShape::MakeSphere(1000.0f),
		Params
	);
	

	if (bHasHit)
	{
		for (auto& Overlap : Overlaps)
		{
			if (AActor* HitActor = Overlap.GetActor())
			{
				UE_LOG(LogTemp, Warning, TEXT("Found enemy: %s"), *HitActor->GetName());

				bIsSpellOnCooldown = true;

				StartCooldownResetTimer();
				
				for (int32 i = 0; i < 3; i++)
				{
					FVector SpawnLocation = PlayerPawn->GetActorLocation();
					FRotator SpawnRotation = (HitActor->GetActorLocation() - SpawnLocation).Rotation();

					AMagicMissileProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AMagicMissileProjectile>(
						ProjectileClass,
						SpawnLocation,
						SpawnRotation
					);

					if (SpawnedProjectile)
					{
						SpawnedProjectile->SetTarget(HitActor);
					}
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No enemies found"));
	}
}

void AMagicMissile::StartCooldownResetTimer()
{
	Super::StartCooldownResetTimer();
}
