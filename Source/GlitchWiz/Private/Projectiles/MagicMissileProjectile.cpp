// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/MagicMissileProjectile.h"

void AMagicMissileProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovementComp = FindComponentByClass<UProjectileMovementComponent>();
}

void AMagicMissileProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Direction = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FVector NewVelocity = Direction * Speed;
	ProjectileMovementComp->Velocity = NewVelocity;
}
