// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/IceballProjectile.h"
#include "Engine/OverlapResult.h"
#include "Interfaces/Enemy.h"

void AIceballProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bShouldApplySlowEffect)
	{
		
	}
}

void AIceballProjectile::OnProjectileHit(FVector Location)
{
	bShouldApplySlowEffect = true;
	TArray<FOverlapResult> Overlaps;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(200);
	
	bool bHit = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		Location,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel2),
		CollisionShape
	);

	if (bDebugMode)
	{
		DrawDebugCircle(
		GetWorld(),
		Location,
		200,
		32,
		FColor::Green,
		false,
		2.0f,
		0,
		5.0f,
		FVector(1,0,0),
		FVector(0,1,0),
		false
		);
	}
	
	if (bHit)
	{
		for (const FOverlapResult& Overlap : Overlaps)
		{
			AActor* HitActor = Overlap.GetActor();
			if (!HitActor) continue;

			if (HitActor->Implements<UEnemy>())
			{
				if (IEnemy* Enemy = Cast<IEnemy>(HitActor))
				{
					Enemy->ApplySlowEffect(0.4f, 3.0f);
				}
			}
		}
	}
}


