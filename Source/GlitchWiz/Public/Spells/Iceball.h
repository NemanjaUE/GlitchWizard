// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/IceballProjectile.h"
#include "Spells/SpellsBase.h"
#include "Iceball.generated.h"

/**
 * 
 */
UCLASS()
class GLITCHWIZ_API AIceball : public ASpellsBase
{
	GENERATED_BODY()

	virtual ESpell GetSpellEnum() const override { return ESpell::Iceball;	}
	
	virtual void PerformSpell() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AIceballProjectile> ProjectileClass;

	virtual void StartCooldownResetTimer() override;
};
