// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
AMouseCharacter::AMouseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MouseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MouseMesh"));
	SetRootComponent(MouseMesh);
	MouseMesh->SetRelativeScale3D(FVector(0.1, 0.1, 0.1));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeLocation(FVector(100.f, 100.f, 100.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->SetRelativeLocation(FVector(-1000.f, 0.f, 200.f));
	Camera->SetRelativeRotation(FRotator3d(-10.f, 0.f, 0.f));

	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 1;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = true;

	isSprinting = false;
	playerVelocity = FVector::ZeroVector;
	terminalVelocity = -40;
	maxHorizontalVelocity = 100;
	isJumping = false;
	prevVelocity = FVector::ZeroVector;
	lives = 5;
	Stamina = 100;
	exhausted = false;
	ArrowCountOfAmmunitionForBow = 0;
}

// Called when the game starts or when spawned
void AMouseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* controller = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* sub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(controller->GetLocalPlayer())) {
			sub->AddMappingContext(InputMapping, 0);
		}
	}
	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = true;
	isSprinting = false;
	playerVelocity = FVector::ZeroVector;
	terminalVelocity = -40;
	maxHorizontalVelocity = 100;
	isJumping = false;
	prevVelocity = FVector::ZeroVector;
	lives = 5;
	Stamina = 100;
	exhausted = false;
}

// Called every frame
void AMouseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlayer(DeltaTime);
	if (isOnGround()) {
		playerVelocity.Z = 0;
		MouseMesh->SetRelativeLocation(FVector(MouseMesh->GetRelativeLocation().X, MouseMesh->GetRelativeLocation().Y, 40));
		isJumping = false;
		playerVelocity = FVector(0, 0, playerVelocity.Z);
		prevVelocity = playerVelocity;
	}
	else {
		playerVelocity.X = prevVelocity.X;
		playerVelocity.Y = prevVelocity.Y;
		playerVelocity *= FVector(0.9, 0.9, 1);
	}
	if (playerVelocity.Z <= terminalVelocity) {
		playerVelocity.Z = -9.8F;
	}
	Camera->SetRelativeLocation(isSprinting ? FVector(-1300, 0, 300) : FVector(-1000, 0, 200));
	ManageStamina();
}

// Called to bind functionality to input
void AMouseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EIC->BindAction(IA_MoveStraight, ETriggerEvent::Triggered, this, &AMouseCharacter::MoveStraight);
		EIC->BindAction(IA_MoveSide, ETriggerEvent::Triggered, this, &AMouseCharacter::MoveSide);
		EIC->BindAction(IA_RotateX, ETriggerEvent::Triggered, this, &AMouseCharacter::RotateX);
		EIC->BindAction(IA_RotateY, ETriggerEvent::Triggered, this, &AMouseCharacter::RotateY);
		EIC->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &AMouseCharacter::Sprint);
		EIC->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &AMouseCharacter::StopSprint);
		EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &AMouseCharacter::JumpUp);
		EIC->BindAction(IA_Crouch, ETriggerEvent::Started, this, &AMouseCharacter::CrouchDown);
		EIC->BindAction(IA_Crouch, ETriggerEvent::Completed, this, &AMouseCharacter::CrouchComplete);
	}
}

void AMouseCharacter::MoveStraight(const FInputActionValue& InputValue) {
	float val = InputValue.Get<float>();
	if (val > 0) {
		float newVelocity = val * isSprinting ? 30 : 10;
		newVelocity *= isJumping ? 1.1 : 1;
		playerVelocity.X += newVelocity;
		prevVelocity.X = playerVelocity.X;
	}
	if (val < 0) {
		float newVelocity = val * 10;
		playerVelocity.X += newVelocity;
		prevVelocity.X = playerVelocity.X;
	}
}

void AMouseCharacter::MoveSide(const FInputActionValue& InputValue) {
	float val = InputValue.Get<float>();
	if (val != 0) {
		playerVelocity.Y += val * 10;
		prevVelocity.Y = playerVelocity.Y;
	}
}

void AMouseCharacter::RotateX(const FInputActionValue& InputValue) {
	float val = InputValue.Get<float>();

	AddControllerYawInput(val);
}

void AMouseCharacter::RotateY(const FInputActionValue& InputValue) {
	float val = InputValue.Get<float>();

	AddControllerPitchInput(val);
}

void AMouseCharacter::Sprint(const FInputActionValue& InputValue) {
	bool val = InputValue.Get<bool>();
	if (val && isOnGround() && Stamina > 0 && !exhausted) {
		this->isSprinting = val;
	}
	else if (Stamina <= 0) {
		this->isSprinting = false;
	}
}

void AMouseCharacter::StopSprint(const FInputActionValue& InputValue) {
	bool val = InputValue.Get<bool>();
	this->isSprinting = false;
}

void AMouseCharacter::JumpUp(const FInputActionValue& InputValue) {
	bool val = InputValue.Get<bool>();
	if (val && isOnGround() && Stamina > 30) {
		MouseMesh->SetRelativeLocation(MouseMesh->GetRelativeLocation() + FVector(0, 0, 110));
		playerVelocity.Z = 125;
		prevVelocity.Z = playerVelocity.Z;
		isJumping = true;
		Stamina -= 30;
	}
}

void AMouseCharacter::CrouchDown(const FInputActionValue& InputValue) {
	bool val = InputValue.Get<bool>();
	if (val) {
		Camera->SetRelativeLocation(FVector(-900.f, 0.f, 150.f));
		Camera->SetRelativeRotation(FRotator3d(-10.f, 0.f, 0.f));
	}
}

void AMouseCharacter::CrouchComplete(const FInputActionValue& InputValue) {
	bool val = InputValue.Get<bool>();
	Camera->SetRelativeLocation(FVector(-1000.f, 0.f, 200.f));
	Camera->SetRelativeRotation(FRotator3d(-10.f, 0.f, 0.f));
}


void AMouseCharacter::MovePlayer(float tickdelta) {
	appplyGravity(10);
	if (FMath::Abs(playerVelocity.X) >= maxHorizontalVelocity) {
		this->playerVelocity.X = maxHorizontalVelocity;
	}
	if (FMath::Abs(playerVelocity.Y) >= maxHorizontalVelocity) {
		this->playerVelocity.Y = maxHorizontalVelocity;
	}
	MouseMesh->SetRelativeLocation(MouseMesh->GetRelativeLocation() + this->playerVelocity * 10.F * tickdelta);
}

void AMouseCharacter::appplyGravity(float strenght) {
	playerVelocity.Z -= strenght;
}

void AMouseCharacter::ManageStamina() {
	if (isSprinting) {
		Stamina--;
	}
	else {
		Stamina++;
	}
	if (Stamina <= 0) {
		exhausted = true;
	}
	if (Stamina >= 20) {
		exhausted = false;
	}
	/*UE_LOG(LogTemp, Warning, TEXT("IA_Forward triggered"));*/
}

bool AMouseCharacter::isOnGround() {
	return MouseMesh->GetRelativeLocation().Z <= 40;
}

float AMouseCharacter::GetXVelocity() {
	return this->playerVelocity.X;
}

float AMouseCharacter::GetYVelocity() {
	return this->playerVelocity.Y;
}

float AMouseCharacter::GetZVelocity() {
	return this->playerVelocity.Z;
}

