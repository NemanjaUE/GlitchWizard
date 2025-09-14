// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spells/SpellsBase.h"
#include "TPose.generated.h"

/**
 * 
 */
UCLASS()
class GLITCHWIZ_API ATPose : public ASpellsBase
{
	GENERATED_BODY()

protected:
	virtual ESpell GetSpellEnum() const override { return ESpell::TPose;	}
	
	virtual void PerformSpell() override;

	virtual void StartCooldownResetTimer() override;

	FTimerHandle TPoseActiveResetTimer;
};
