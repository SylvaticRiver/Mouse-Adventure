 // Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MouseMesh"));
	EnemyMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* controller = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* sub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(controller->GetLocalPlayer())) {
			sub->AddMappingContext(InputMapping, 0);
		}
	}
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EIC->BindAction(IA_MoveStraight, ETriggerEvent::Triggered, this, &ABaseEnemy::MoveStraight);
		EIC->BindAction(IA_MoveSide, ETriggerEvent::Triggered, this, &ABaseEnemy::MoveSide);
	}
}

void ABaseEnemy::MoveStraight(const FInputActionValue& InputValue) {
	float val = InputValue.Get<float>();
	if (val > 0) {
		AddMovementInput(UKismetMathLibrary::GetForwardVector(Controller->GetControlRotation()));
	}
}

void ABaseEnemy::MoveSide(const FInputActionValue & InputValue) {
	float val = InputValue.Get<float>();
	if (val != 0) {
		AddMovementInput(UKismetMathLibrary::GetRightVector(Controller->GetControlRotation()));
	}
}

