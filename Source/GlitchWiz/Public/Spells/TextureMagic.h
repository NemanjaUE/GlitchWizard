// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spells/SpellsBase.h"
#include "TextureMagic.generated.h"

/**
 * 
 */
UCLASS()
class GLITCHWIZ_API ATextureMagic : public ASpellsBase
{
	GENERATED_BODY()

protected:
	virtual ESpell GetSpellEnum() const override { return ESpell::TextureMagic;	}
	
	virtual void PerformSpell() override;

	virtual void StartCooldownResetTimer() override;

	FTimerHandle TextureMagicActiveResetTimer;
};
