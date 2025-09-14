// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/MagicMissileProjectile.h"

#include "Characters/Enemies/Corruptor/Corruptor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/CharacterMovementComponent.h"

AMagicMissileProjectile::AMagicMissileProjectile()
{
	
}

void AMagicMissileProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovementComp = FindComponentByClass<UProjectileMovementComponent>();

	FindComponentByClass<UStaticMeshComponent>()->OnComponentBeginOverlap.AddDynamic(this, &AMagicMissileProjectile::OnOverlapBegin);
}

void AMagicMissileProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(TargetActor))
	{
		AcquireNewTarget();
		if (!IsValid(TargetActor))
		{
			Destroy();
			return;
		}
	}

	FVector Direction = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	ProjectileMovementComp->Velocity = Direction * Speed;
}

void AMagicMissileProjectile::AcquireNewTarget()
{
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHasHit = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		GetActorLocation(),
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel2),
		FCollisionShape::MakeSphere(1000.f),
		Params
	);

	if (bHasHit)
	{
		AActor* Closest = nullptr;
		float BestDistSqr = TNumericLimits<float>::Max();

		for (auto& Overlap : Overlaps)
		{
			if (AActor* Candidate = Overlap.GetActor())
			{
				float DistSqr = FVector::DistSquared(Candidate->GetActorLocation(), GetActorLocation());
				if (DistSqr < BestDistSqr)
				{
					BestDistSqr = DistSqr;
					Closest = Candidate;
				}
			}
		}
		TargetActor = Closest;
	}
	else
	{
		TargetActor = nullptr;
	}
}

void AMagicMissileProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (PlayerPawn->bIsTPoseSpellActive)
	{
		UGameplayStatics::ApplyDamage(OtherActor, 35, GetInstigatorController(), this, UDamageType::StaticClass());
		
		USkeletalMeshComponent* SkeletalMeshComp = OtherActor->FindComponentByClass<USkeletalMeshComponent>();
		if (SkeletalMeshComp)
		{
			SkeletalMeshComp->SetAnimationMode(EAnimationMode::Type::AnimationCustomMode);
			Cast<ACorruptor>(OtherActor)->GetCharacterMovement()->DisableMovement();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid mesh"));
		}


		FTimerHandle EndTPoseHandle;
		GetWorldTimerManager().SetTimer(EndTPoseHandle, [OtherActor]()
		{
			OtherActor->FindComponentByClass<USkeletalMeshComponent>()->SetAnimationMode(EAnimationMode::Type::AnimationBlueprint);
			Cast<ACorruptor>(OtherActor)->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}, 5.0f, false);
	}

	if (PlayerPawn->bIsNoclipSpellActive)
	{
		UGameplayStatics::ApplyDamage(OtherActor, 35, GetInstigatorController(), this, UDamageType::StaticClass());
		UGameplayStatics::ApplyDamage(OtherActor, 35, GetInstigatorController(), this, UDamageType::StaticClass());
	}

	if (PlayerPawn->bIsTextureMagicSpellActive)
	{
		if (OtherActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing texture projectile"));
			UGameplayStatics::ApplyDamage(OtherActor, 20, GetInstigatorController(), this, UDamageType::StaticClass());
			FTimerHandle DamageOverTimeHandle;
			GetWorldTimerManager().SetTimer(DamageOverTimeHandle, [OtherActor, this]() { UGameplayStatics::ApplyDamage(OtherActor, 2, GetInstigatorController(), this, UDamageType::StaticClass()); }, 0.5, true);

			FTimerHandle EndOverTimeDamageHandle;
			GetWorldTimerManager().SetTimer(EndOverTimeDamageHandle, [this, DamageOverTimeHandle]() { GetWorldTimerManager().PauseTimer(DamageOverTimeHandle); }, 5.0f, false);
		}
	}

	else
	{
		UGameplayStatics::ApplyDamage(OtherActor, 35, GetInstigatorController(), this, UDamageType::StaticClass());
	}
	
	Destroy();
}

