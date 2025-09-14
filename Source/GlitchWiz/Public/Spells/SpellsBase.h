// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/ESpell.h"
#include "Components/Systems/InventoryComponent.h"
#include "Interfaces/Pickupable.h"
#include "Camera/CameraComponent.h"
#include "SpellsBase.generated.h"

UCLASS()
class GLITCHWIZ_API ASpellsBase : public AActor, public IPickupable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpellsBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UInventoryComponent* InventoryComp;
	
	virtual void PickUp();

	UFUNCTION(BlueprintCallable)
	virtual void PerformSpell();

	virtual void AddSpellToInventory(ESpell SpellToAdd);

public:
	UPROPERTY(EditAnywhere)
	float SpellDamage;

	bool bCanBeEquipped = true;

	UFUNCTION(BlueprintCallable)
	virtual ESpell GetSpellEnum() const;

public:
	APawn* PlayerPawn;
	UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly)
	float SpellCooldown;

	UPROPERTY(VisibleAnywhere)
	bool bIsSpellOnCooldown = false;

	FTimerHandle CooldownResetTimer;
	virtual void StartCooldownResetTimer();

	
	UPROPERTY(EditDefaultsOnly)
	float SpellLength;
	
	static bool bIsNoclipSpellActive;
	static bool bIsTPoseSpellActive;
};
