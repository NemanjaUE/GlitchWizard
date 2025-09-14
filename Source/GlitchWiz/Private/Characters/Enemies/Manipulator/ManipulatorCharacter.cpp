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
		if (bInAttackRange)
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

void AManipulatorCharacter::ApplyNoclipIceballEffect()
{
}

void AManipulatorCharacter::ApplyTPoseIceballEffect()
{
}

void AManipulatorCharacter::ApplyIceballSlowEffect(float Percent, float Duration)
{
}

void AManipulatorCharacter::ApplyTextureMagicIceballEffect()
{
}

void AManipulatorCharacter::ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent,
	APawn* PawnInstigator, AActor* DamageCauser)
{
	Super::ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
}





