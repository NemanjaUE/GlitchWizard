// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spells/SpellsBase.h"
#include "MagicMissile.generated.h"

/**
 * 
 */
UCLASS()
class GLITCHWIZ_API AMagicMissile : public ASpellsBase
{
	GENERATED_BODY()

protected:
	virtual ESpell GetSpellEnum() const override { return ESpell::MagicMissile;	}
	
	virtual void PerformSpell() override;
};
