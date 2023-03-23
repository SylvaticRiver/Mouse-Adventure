// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MouseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MouseMesh"));
	SetRootComponent(MouseMesh);
	MouseMesh->SetRelativeScale3D(FVector(0.1, 0.1, 0.1));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeLocation(FVector(100.f, 100.f, 100.f));
	SpringArm->TargetArmLength = 400.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->SetRelativeLocation(FVector(-1000.f, 0.f, 200.f));
	Camera->SetRelativeRotation(FRotator3d(-10.f, 0.f, 0.f));

	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 1;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	isSprinting = false;
	playerVelocity = FVector::ZeroVector;
	terminalVelocity = -40;
	isJumping = false;
	prevVelocity = FVector::ZeroVector;
	lives = 5;
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* controller = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* sub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(controller->GetLocalPlayer())) {
			sub->AddMappingContext(InputMapping, 0);
		}
	}

	isSprinting = false;
	playerVelocity = FVector::ZeroVector;
	terminalVelocity = -40;
	isJumping = false;
	prevVelocity = FVector::ZeroVector;
	lives = 5;
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlayer();
	if (isOnGround()) {
		playerVelocity.Z = 0;
		isJumping = false;
		playerVelocity = FVector(0, 0, playerVelocity.Z);
		prevVelocity = playerVelocity;
	}
	else {
		playerVelocity.X = prevVelocity.X;
		playerVelocity.Y = prevVelocity.Y;

	}
	if (playerVelocity.Z <= terminalVelocity) {
		playerVelocity.Z = -40;
	}
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EIC->BindAction(IA_MoveStraight, ETriggerEvent::Triggered, this, &AMainPlayer::MoveStraight);
		EIC->BindAction(IA_MoveSide, ETriggerEvent::Triggered, this, &AMainPlayer::MoveSide);
		EIC->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &AMainPlayer::Sprint);
		EIC->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &AMainPlayer::StopSprint);
		EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &AMainPlayer::Jump);
		EIC->BindAction(IA_Crouch, ETriggerEvent::Started, this, &AMainPlayer::Crouch);
		EIC->BindAction(IA_Crouch, ETriggerEvent::Completed, this, &AMainPlayer::CrouchComplete);
	}
}

void AMainPlayer::MoveStraight(const FInputActionValue& InputValue) {
	float val = InputValue.Get<float>();
	if (val > 0) {
		float newVelocity = val * isSprinting ? 30 : 10;
		newVelocity *= isJumping ? 1.25 : 1;
		playerVelocity.X += newVelocity;
		prevVelocity.X = playerVelocity.X;
	}
	if (val < 0) {
		float newVelocity = val * 10;
		playerVelocity.X += newVelocity;
		prevVelocity.X = playerVelocity.X;
	}
}

void AMainPlayer::MoveSide(const FInputActionValue& InputValue) {
	float val = InputValue.Get<float>();
	if (val != 0) {
		playerVelocity.Y += val * 10;
		prevVelocity.Y = playerVelocity.Y;
	}
}

void AMainPlayer::Sprint(const FInputActionValue& InputValue) {
	bool val = InputValue.Get<bool>();
	if (val && isOnGround()) {
		this->isSprinting = val;
	}
}

void AMainPlayer::StopSprint(const FInputActionValue& InputValue) {
	bool val = InputValue.Get<bool>();
	this->isSprinting = false;
}

void AMainPlayer::Jump(const FInputActionValue& InputValue) {
	bool val = InputValue.Get<bool>();
	if (val && isOnGround()) {
		MouseMesh->SetRelativeLocation(MouseMesh->GetRelativeLocation() + FVector(0, 0, 110));
		playerVelocity.Z = 100;
		prevVelocity.Z = playerVelocity.Z;
		isJumping = true;
	}
}

void AMainPlayer::Crouch(const FInputActionValue& InputValue) {
	bool val = InputValue.Get<bool>();
	if (val) {
		Camera->SetRelativeLocation(FVector(-900.f, 0.f, 150.f));
		Camera->SetRelativeRotation(FRotator3d(-10.f, 0.f, 0.f));
	}
}

void AMainPlayer::CrouchComplete(const FInputActionValue& InputValue) {
	bool val = InputValue.Get<bool>();
	Camera->SetRelativeLocation(FVector(-1000.f, 0.f, 200.f));
	Camera->SetRelativeRotation(FRotator3d(-10.f, 0.f, 0.f));
}

void AMainPlayer::MovePlayer() {
	appplyGravity(20);
	MouseMesh->SetRelativeLocation(MouseMesh->GetRelativeLocation() + this->playerVelocity);
}

void AMainPlayer::appplyGravity(float strenght) {
	playerVelocity.Z -= strenght;
}

bool AMainPlayer::isOnGround() {
	return MouseMesh->GetRelativeLocation().Z <= 100;
}
