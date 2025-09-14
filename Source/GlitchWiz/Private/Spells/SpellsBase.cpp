// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Spells/SpellsBase.h"
#include "Spells/SpellsBase.h"

// Sets default values
ASpellsBase::ASpellsBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpellsBase::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	InventoryComp = PlayerPawn->FindComponentByClass<UInventoryComponent>();
	CameraComp = PlayerPawn->FindComponentByClass<UCameraComponent>();
}

// Called every frame
void ASpellsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpellsBase::PickUp()
{
	AddSpellToInventory(GetSpellEnum());

	InventoryComp->StoreSpellActor(GetSpellEnum(), this);
	bCanBeEquipped = true;
	SetActorHiddenInGame(true);
}

void ASpellsBase::AddSpellToInventory(ESpell SpellToAdd)
{
	InventoryComp->AddSpellToInventory(SpellToAdd, 1);
}

ESpell ASpellsBase::GetSpellEnum() const
{
	return ESpell::None;
}

void ASpellsBase::StartCooldownResetTimer()
{
	GetWorldTimerManager().SetTimer(CooldownResetTimer, [this]() { bIsSpellOnCooldown = false; }, SpellCooldown, false);
}

void ASpellsBase::PerformSpell()
{
	UE_LOG(LogTemp, Warning, TEXT("Performing equipped spell"));
}
