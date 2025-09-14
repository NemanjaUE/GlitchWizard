


#include "Characters/Enemies/Corruptor/Corruptor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Controllers/Corruptor/CorruptorAIController.h"


ACorruptor::ACorruptor()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_CorruptorObj(TEXT("/Game/AI/Corruptor/BT_Corruptor.BT_Corruptor"));
	if (BT_CorruptorObj.Succeeded())
	{
		CustomBehaviorTree = BT_CorruptorObj.Object;
	}
}
void ACorruptor::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);
	if (ACharacter* PlayerChar = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		if (UCharacterMovementComponent* MoveComp = PlayerChar->GetCharacterMovement())
		{
			OriginalPlayerSpeed = MoveComp->MaxWalkSpeed;
			UE_LOG(LogTemp, Warning, TEXT("OriginalPlayerSpeed: %f"), OriginalPlayerSpeed);
		}
	}
}


void ACorruptor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ACorruptorAIController* CorruptorAiController = Cast<ACorruptorAIController>(GetController());
	if (!CorruptorAiController) return;
	UBlackboardComponent* Blackboard = CorruptorAiController->GetBlackboardComponent();
	if (!Blackboard) return;
	AActor* Target = CorruptorAiController->CurrentTarget;
	if (Target)
	{
		float Distance = GetDistanceTo(Target);
		bool bInAttackRange = Distance <= AttackRange;
		if (bInAttackRange)
		{
			CorruptorAiController->SetState("Attack");
		}
		else
		{
			CorruptorAiController->SetState("Chase");
		}
		
	}
	else
	{
		CorruptorAiController->SetState("Idle");
	}
	if (bIsAttacking == true && bCanTraceAttack == true)
	{
		PerformAttackTrace();
	}
}

UBehaviorTree* ACorruptor::GetBehaviorTree() const
{
	return CustomBehaviorTree;
}


float ACorruptor::GetMovementSpeed() const
{
	if (GetCharacterMovement())
	{
		return GetCharacterMovement()->Velocity.Size();
	}
	return 0.0f;
}

void ACorruptor::StartAttack()
{
	ACorruptorAIController* CorruptorAiController = Cast<ACorruptorAIController>(GetController());
	if (!CorruptorAiController) return;
	AActor* Target = CorruptorAiController->CurrentTarget;
	if (Target && GetDistanceTo(Target) <= AttackRange)
	{
		bIsAttacking = true;
		if (AttackMontage && GetMesh() && GetMesh()->GetAnimInstance())
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Stop(0.1f);
			AnimInstance->Montage_Play(AttackMontage, 1.0f);
			
			
		}
	}
	else
	{
		bIsAttacking = false;
	}
}

void ACorruptor::PerformAttackTrace()
{
	if (!GetMesh()) return;
	FVector Start = GetMesh()->GetSocketLocation(AttackStartSocket);
	FVector End = GetMesh()->GetSocketLocation(AttackEndSocket);
	FQuat Rot = GetMesh()->GetSocketQuaternion(AttackStartSocket);
	float Lenght = FVector::Distance(Start, End);
	FVector HalfSize (AttackBoxHalfWidth, AttackBoxHalfHeight, Lenght * 0.5f);
	FVector Center = FMath::Lerp(Start, End, 0.5f);
	TArray<FHitResult> Hits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	GetWorld()->SweepMultiByChannel(Hits, Start, End, Rot, ECC_GameTraceChannel5, FCollisionShape::MakeBox(HalfSize), Params);
	for (auto& Hit : Hits)
	{
		bHasHitTarget = true;
		int32 StencilValue = 1;
		AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (ACharacter* Char = Cast<ACharacter>(Player))
		{
			if (UCharacterMovementComponent* MoveComp = Char->GetCharacterMovement())
			{
				float SlowedMoveSpeed = MoveComp->MaxWalkSpeed = OriginalPlayerSpeed * 0.5f;
				MoveComp->MaxWalkSpeed = SlowedMoveSpeed;
				UE_LOG(LogTemp, Warning, TEXT("%f"), SlowedMoveSpeed);
			}
		}
		for (AActor* Actor : AllActors)
		{
			
			if (Actor == Player) continue;

			TArray<UActorComponent*> Components;
			Actor->GetComponents(UPrimitiveComponent::StaticClass(), Components);
			for (UActorComponent* Comp : Components)
			{
				if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Comp))
				{
					PrimComp->SetRenderCustomDepth(true);
					PrimComp->SetCustomDepthStencilValue(StencilValue);
				}
			}
		}
	}
	DrawDebugBox(GetWorld(), Center, HalfSize, Rot, FColor::Red, false, 0.2);
	FTimerHandle ResetHandle;
	GetWorld()->GetTimerManager().SetTimer(
	ResetStencilHandle,
	this,
	&ACorruptor::ResetStencil,
	3.0f,
	false
	);

}



void ACorruptor::ResetStencil()
{
	int32 ResetValue = 0;

	AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	for (AActor* Actor : AllActors)
	{
		if (Actor == Player) continue;

		TArray<UActorComponent*> Components;
		Actor->GetComponents(UPrimitiveComponent::StaticClass(), Components);

		for (UActorComponent* Comp : Components)
		{
			if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Comp))
			{
				PrimComp->SetRenderCustomDepth(false);
				PrimComp->SetCustomDepthStencilValue(ResetValue);
			}
		}
	}
	if (ACharacter* Char = Cast<ACharacter>(Player))
	{
		if (UCharacterMovementComponent* MoveComp = Char->GetCharacterMovement())
		{
			MoveComp->MaxWalkSpeed = OriginalPlayerSpeed;
			UE_LOG(LogTemp, Warning, TEXT("%f"), OriginalPlayerSpeed);
		}
	}
}

void ACorruptor::ApplySlowEffect(float Percent, float Duration)
{
	float OriginalSpeed = GetCharacterMovement()->MaxWalkSpeed;
	float NewSpeed = OriginalSpeed * (1.f - Percent);
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

	FTimerHandle ResetSlowTimer;
	GetWorldTimerManager().SetTimer(ResetSlowTimer, [this]() { GetCharacterMovement()->MaxWalkSpeed = 600.0f; }, Duration, false);
}

