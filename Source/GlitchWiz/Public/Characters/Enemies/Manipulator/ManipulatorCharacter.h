// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "Interfaces/Enemy.h"
#include "ManipulatorCharacter.generated.h"

UENUM(BlueprintType)
enum class EManipulatorState : uint8
{
	Idle,
	Chase,
	Attack,
};

UCLASS()
class GLITCHWIZ_API AManipulatorCharacter : public ACharacter, public IEnemy
{
	GENERATED_BODY()

public:
	
	AManipulatorCharacter();

protected:

	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;
	virtual UBehaviorTree* GetBehaviorTree() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Manipulator")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Manipulator")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* CustomBehaviorTree;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	float GetMovementSpeed() const;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Manipulator")
	EManipulatorState ManipulatorState = EManipulatorState::Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Manipulator")
	bool bIsAttacking = false;

	void StartAttack();

	UPROPERTY(EditAnywhere, Category="Attack")
	FName AttackStartSocket;
	UPROPERTY(EditAnywhere, Category="Attack")
	FName AttackEndSocket;
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SpawnProjectile();
	UPROPERTY(VisibleAnywhere)
	bool bCanAttack = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float ProjectileSpeed = 2000.f;
	


private:
	float OriginalPlayerSpeed;
	virtual void ApplyNoclipIceballEffect() override;
	virtual void ApplyTPoseIceballEffect() override;
	virtual void ApplyIceballSlowEffect(float Percent, float Duration) override;
	virtual void ApplyTextureMagicIceballEffect() override;
	virtual void ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser) override;
};
