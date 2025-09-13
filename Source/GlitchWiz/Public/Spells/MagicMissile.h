// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spells/SpellsBase.h"
#include "Projectiles/MagicMissileProjectile.h"
#include "MagicMissile.generated.h"

class AMagicMissileProjectile;
/**
 * 
 */
UCLASS()
class GLITCHWIZ_API AMagicMissile : public ASpellsBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual ESpell GetSpellEnum() const override { return ESpell::MagicMissile;	}
	
	virtual void PerformSpell() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicMissileProjectile> ProjectileClass;

	virtual void StartCooldownResetTimer() override;
};
