// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spells/SpellsBase.h"
#include "Noclip.generated.h"

/**
 * 
 */
UCLASS()
class GLITCHWIZ_API ANoclip : public ASpellsBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual ESpell GetSpellEnum() const override { return ESpell::Noclip; }
	
	virtual void PerformSpell() override;

	virtual void StartCooldownResetTimer() override;

	FTimerHandle NoclipActiveResetTimer;

public:
	
};
