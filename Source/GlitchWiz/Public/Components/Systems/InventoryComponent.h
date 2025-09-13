// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/ESpell.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLITCHWIZ_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	TMap<ESpell, int32> SpellMap;
	TMap<ESpell, class ASpellsBase*> SpellActorMap;

	void AddSpellToInventory(ESpell SpellID, int32 Amount);
	void RemoveSpellFromInventory(ESpell SpellID, int32 Amount);
	void StoreSpellActor(ESpell SpellID, class ASpellsBase* SpellActor);

	UFUNCTION(BlueprintCallable)
	void EquipSpell(ESpell SpellToEquip);

	UFUNCTION(BlueprintCallable)
	void UnequipSpell(ESpell SpellToUnequip);

	UPROPERTY(BlueprintReadWrite)
	ASpellsBase* EquippedSpell;
};
