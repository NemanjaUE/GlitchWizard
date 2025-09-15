#include "Characters/Enemies/Manipulator/ManipulatorCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Controllers/Manipulator/ManipulatorAIController.h"
#include "Runtime/Engine/Internal/Kismet/BlueprintTypeConversions.h"

AManipulatorCharacter::AManipulatorCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_ManipulatorObj(TEXT("/Game/AI/Manipulator/BT_Manipulator.BT_Manipulator"));
	if (BT_ManipulatorObj.Succeeded())
	{
		CustomBehaviorTree = BT_ManipulatorObj.Object;
	}
}

void AManipulatorCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (ACharacter* PlayerChar = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		if (UCharacterMovementComponent* MoveComp = PlayerChar->GetCharacterMovement())
		{
			OriginalPlayerSpeed = MoveComp->MaxWalkSpeed;
			UE_LOG(LogTemp, Warning, TEXT("OriginalPlayerSpeed: %f"), OriginalPlayerSpeed);
		}
	}
}

void AManipulatorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AManipulatorAIController* ManipulatorAiController = Cast<AManipulatorAIController>(GetController());
	if (!ManipulatorAiController) return;
	UBlackboardComponent* Blackboard = ManipulatorAiController->GetBlackboardComponent();
	if (!Blackboard) return;
	AActor* Target = ManipulatorAiController->CurrentTarget;
	if (Target)
	{
		float Distance = GetDistanceTo(Target);
		float MinAttackDistance = 100.f;
		float MaxAttackDistance = 700.f;
		FVector End = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), End);
		SetActorRotation(LookAtRotation);
		bool bInAttackRange = Distance >= MinAttackDistance && Distance <= MaxAttackDistance;
		if (bInAttackRange && bIsAttackEnabled)
		{
			ManipulatorAiController->SetState("Attack");
		}
		else
		{
			ManipulatorAiController->SetState("Chase");
		}
	}
	else
	{
		ManipulatorAiController->SetState("Idle");
	}

	if  (bShouldFall == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Should Fall"));
		FVector CurrentLocation = GetActorLocation();

		CurrentLocation.Z -= 100 * DeltaTime;

		SetActorLocation(CurrentLocation);
	}
}

UBehaviorTree* AManipulatorCharacter::GetBehaviorTree() const
{
	return CustomBehaviorTree;
}

float AManipulatorCharacter::GetMovementSpeed() const
{
	if (GetCharacterMovement())
	{
		return GetCharacterMovement()->Velocity.Size();
	}
	return 0.0f;
}

void AManipulatorCharacter::StartAttack()
{
	if (bIsAttacking) return;
	AManipulatorAIController* ManipulatorAiController = Cast<AManipulatorAIController>(GetController());
	if (!ManipulatorAiController) return;
	AActor* Target = ManipulatorAiController->CurrentTarget;
	if (Target && GetDistanceTo(Target) <= AttackRange)
	{
		bIsAttacking = true;
		if (AttackMontage && GetMesh() && GetMesh()->GetAnimInstance())
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			UE_LOG(LogTemp, Warning, TEXT("Playing Attack Montage"));
			AnimInstance->Montage_Stop(0.1f);
			AnimInstance->Montage_Play(AttackMontage, 1.0f);
		}
	}
	else
	{
		bIsAttacking = false;
	}
}

void AManipulatorCharacter::SpawnProjectile()
{
	if (!ProjectileClass || !GetMesh()) return;

	FVector Start = GetMesh()->GetSocketLocation(AttackStartSocket);
	FVector End = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector Direction = (End - Start).GetSafeNormal();
	FRotator SpawnRotation = Direction.Rotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(
		ProjectileClass,
		Start,
		SpawnRotation,
		SpawnParams
	);

	if (SpawnedProjectile)
	{
		if (UProjectileMovementComponent* ProjMove = SpawnedProjectile->FindComponentByClass<UProjectileMovementComponent>())
		{
			ProjMove->Velocity = Direction * ProjectileSpeed;
		}
	}
}

void AManipulatorCharacter::ApplyIceballSlowEffect(float Percent, float Duration)
{
	float OriginalSpeed = GetCharacterMovement()->MaxWalkSpeed;
	float NewSpeed = OriginalSpeed * (1.f - Percent);
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

	FTimerHandle ResetSlowTimer;
	GetWorldTimerManager().SetTimer(ResetSlowTimer, [this]() { GetCharacterMovement()->MaxWalkSpeed = 600.0f; }, Duration, false);
}

void AManipulatorCharacter::ApplyNoclipIceballEffect()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->Deactivate();

	FTimerHandle DestroyTimerManipulator;
	GetWorldTimerManager().SetTimer(DestroyTimerManipulator, [this]() { Destroy(); }, 5.0f, false);
	
	bShouldFall = true;
}

void AManipulatorCharacter::ApplyTPoseIceballEffect()
{
	bIsAttackEnabled = false;

	FindComponentByClass<USkeletalMeshComponent>()->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode);
	GetCharacterMovement()->StopMovementImmediately();
	
	FTimerHandle EnableAttackTimer;
	GetWorldTimerManager().SetTimer(EnableAttackTimer, [this]()
	{
		bIsAttackEnabled = true;
		FindComponentByClass<USkeletalMeshComponent>()->SetAnimationMode(EAnimationMode::Type::AnimationBlueprint);
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}, 5.0f, false);
}

void AManipulatorCharacter::ApplyTextureMagicIceballEffect()
{
	GetCharacterMovement()->MaxWalkSpeed = 0;

	FTimerHandle ResetStunTimer;
	GetWorldTimerManager().SetTimer(ResetStunTimer, [this]() { GetCharacterMovement()->MaxWalkSpeed = 600.0f; }, 5.0f, false);
}





