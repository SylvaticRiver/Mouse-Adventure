// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EngineUtils.h"
#include "Arrow.h"
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
	MouseMesh->SetupAttachment(GetRootComponent());
	MouseMesh->SetRelativeScale3D(FVector(1.9775, 1.9775, 1.9775));
	MouseMesh->SetRelativeRotation(FRotator(0, 0, -90));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeLocation(FVector(100.f, 100.f, 100.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeLocation(FVector(-1000.f, 0.f, 200.f));
	Camera->SetRelativeRotation(FRotator3d(-10.f, 0.f, 0.f));

	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = false;

	isSprinting = false;
	playerVelocity = FVector::ZeroVector;
	terminalVelocity = -40;
	maxHorizontalVelocity = 100;
	isJumping = false;
	prevVelocity = FVector::ZeroVector;
	lives = 5;
	Stamina = 100;
	exhausted = false;
	bowcharge = 0;
	ArrowCountOfAmmunitionForBow = 0;
	lastCheckpointPos = FVector::ZeroVector;
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
		playerVelocity = FVector::ZeroVector;
		prevVelocity = playerVelocity;
	}
	else {
		playerVelocity.X = prevVelocity.X;
		playerVelocity.Y = prevVelocity.Y;
		playerVelocity *= FVector(0.9, 0.9, 1);
	}
	if (playerVelocity.Z <= terminalVelocity) {
		playerVelocity.Z = -terminalVelocity;
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
		EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &AMouseCharacter::Jump);
		EIC->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AMouseCharacter::JumpUpComplete);
		EIC->BindAction(IA_Crouch, ETriggerEvent::Started, this, &AMouseCharacter::CrouchDown);
		EIC->BindAction(IA_Crouch, ETriggerEvent::Completed, this, &AMouseCharacter::CrouchComplete);
		EIC->BindAction(IA_Bow, ETriggerEvent::Triggered, this, &AMouseCharacter::ChargeBow);
		EIC->BindAction(IA_Bow, ETriggerEvent::Completed, this, &AMouseCharacter::ShootBow);
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
	onDeath();
}

void AMouseCharacter::StopSprint(const FInputActionValue& InputValue) {
	bool val = InputValue.Get<bool>();
	this->isSprinting = false;
}

void AMouseCharacter::JumpUp(const FInputActionValue& InputValue) {
	bool val = InputValue.Get<bool>();
	if (val && isOnGround() && Stamina > 30) {
		UE_LOG(LogTemp, Warning, TEXT("Jumped"));
		isJumping = true;
		playerVelocity.Z = 125;
		prevVelocity.Z = playerVelocity.Z;
		Stamina -= 30;
	}
}

void AMouseCharacter::JumpUpComplete(const FInputActionValue& InputValue) {
	
	/*isJumping = false;*/
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

void AMouseCharacter::ChargeBow(const FInputActionValue& InputValue) {
	bool val = InputValue.Get<bool>();
	if (val && bowcharge < 20) {
		UE_LOG(LogTemp, Warning, TEXT("Charging ticks = %d"), bowcharge);
		Camera->FieldOfView -= 2;
		bowcharge++;
	}
}

void AMouseCharacter::ShootBow(const FInputActionValue& InputValue) {
	bool val = InputValue.Get<bool>();
	float VelocityMultiplier = 0.1 + (float)bowcharge / 120;		
	AActor* spawnedArrow = GetWorld()->SpawnActor<AActor>(Arrow, GetActorLocation() + GetViewRotation().Vector(), GetViewRotation());
	Cast<AArrow>(spawnedArrow)->arrowVelocity = FVector(VelocityMultiplier, VelocityMultiplier * 0.02, VelocityMultiplier);
	bowcharge = 0;
	Camera->FieldOfView = 90;
}

void AMouseCharacter::CattailSling(const FInputActionValue& InputValue) {

}

void AMouseCharacter::MovePlayer(float tickdelta) {
	appplyGravity(10);
	if (FMath::Abs(playerVelocity.X) >= maxHorizontalVelocity) {
		this->playerVelocity.X = this->playerVelocity.X > 0 ? maxHorizontalVelocity : maxHorizontalVelocity * -1;
	}
	if (FMath::Abs(playerVelocity.Y) >= maxHorizontalVelocity) {
		this->playerVelocity.Y = this->playerVelocity.Y > 0 ? maxHorizontalVelocity : maxHorizontalVelocity * -1;
	}
	AddMovementInput(playerVelocity);
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

void AMouseCharacter::onDeath()
{
	playerVelocity = FVector::ZeroVector;
	this->SetActorLocation(lastCheckpointPos);
	isSprinting = false;
	playerVelocity = FVector::ZeroVector;
	isJumping = false;
	prevVelocity = FVector::ZeroVector;
	lives = 5;
	Stamina = 100;
	exhausted = false;
	bowcharge = 0;
}

bool AMouseCharacter::isOnGround() {
	return !GetCharacterMovement()->IsFalling() && !isJumping;
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

