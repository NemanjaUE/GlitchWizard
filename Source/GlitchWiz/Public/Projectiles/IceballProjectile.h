// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/ProjectileBase.h"
#include "IceballProjectile.generated.h"

/**
 * 
 */
UCLASS()
class GLITCHWIZ_API AIceballProjectile : public AProjectileBase
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void OnProjectileHit(FVector Location);
public:
	virtual void Tick(float DeltaTime) override;

private:
	bool bShouldApplySlowEffect = true;

public:
	bool bDebugMode = false;
};
