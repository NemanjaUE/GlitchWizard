// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/EndDoor.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEndDoor::AEndDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEndDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEndDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEndDoor::Interact()
{
	UE_LOG(LogTemp, Display, TEXT("Interacting with end door"));

	UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation());
	
	EndScreenWidget = CreateWidget<UUserWidget>(GetWorld(), EndScreenWidgetClass);
	EndScreenWidget->AddToViewport();
	
	APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	PC->SetInputMode(FInputModeUIOnly());
	PC->bShowMouseCursor = true;
}

