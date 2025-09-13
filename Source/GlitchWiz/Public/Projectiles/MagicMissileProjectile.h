// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectiles/ProjectileBase.h"
#include "MagicMissileProjectile.generated.h"

/**
 * 
 */
UCLASS()
class GLITCHWIZ_API AMagicMissileProjectile : public AProjectileBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaTime) override;

protected:
	UProjectileMovementComponent* ProjectileMovementComp;
	
public:
	UPROPERTY(EditAnywhere)
	float Speed;
	
	UPROPERTY()
	AActor* TargetActor;

	void SetTarget(AActor* InTarget)
	{
		TargetActor = InTarget;
	}
};
