// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EngineUtils.h"
#include "Arrow.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AMouseCharacter::AMouseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MouseMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MouseMesh"));
	MouseMesh->SetupAttachment(GetRootComponent());
	MouseMesh->SetRelativeScale3D(FVector(1.9775, 1.9775, 1.9775));
	MouseMesh->SetRelativeRotation(FRotator(0, 0, -90));
	MouseMesh->SetRelativeLocation(FVector(0, 0, -85));

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

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	isSprinting = false;
	playerVelocity = FVector::ZeroVector;
	terminalVelocity = -40;
	maxHorizontalVelocity = 100000;
	isJumping = false;
	prevVelocity = FVector::ZeroVector;
	lives = 5;
	Stamina = 1000;
	exhausted = false;
	bowcharge = 0;
	ArrowCountOfAmmunitionForBow = 3;
	gnomePiecesCollected = 0;
	invulTicks = 0;
	this->GetCharacterMovement()->MaxWalkSpeed = 500;
	WonGame = false;
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
	maxHorizontalVelocity = 100000;
	isJumping = false;
	prevVelocity = FVector::ZeroVector;
	lastCheckpointPos = GetActorLocation();
	lives = 5;
	Stamina = 1000;
	exhausted = false;
	gnomePiecesCollected = 0;
	invulTicks = 0;
	this->GetCharacterMovement()->MaxWalkSpeed = 500;
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
	if (gnomePiecesCollected >= 2) {
		onWin();
	}

	if (invulTicks > 0) {
		invulTicks--;
	}
	else if (invulTicks < 0) {
		invulTicks = 0;
	}
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
		EIC->BindAction(IA_Bow, ETriggerEvent::Triggered, this, &AMouseCharacter::ChargeBow);
		EIC->BindAction(IA_Bow, ETriggerEvent::Completed, this, &AMouseCharacter::ShootBow);
	}
}

void AMouseCharacter::MoveStraight(const FInputActionValue& InputValue) {
	float val = InputValue.Get<float>();
	if (val > 0) {
		float newVelocity = val * 10;
		newVelocity *= isJumping ? 1.1 : 1;
		playerVelocity.X += newVelocity * (isSprinting ? 10000 : 1);
		prevVelocity.X = playerVelocity.X;
	}
	if (val < 0) {
		float newVelocity = val * 10;
		playerVelocity.X += newVelocity * (isSprinting ? 10000 : 1);
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
	if (val && isOnGround() && Stamina > 300) {
		UE_LOG(LogTemp, Warning, TEXT("Jumped"));
		isJumping = true;
		playerVelocity.Z = 125;
		prevVelocity.Z = playerVelocity.Z;
		Stamina -= 150;
	}
}

void AMouseCharacter::ChargeBow(const FInputActionValue& InputValue) {
	bool val = InputValue.Get<bool>();
	if (ArrowCountOfAmmunitionForBow > 0) {
		if (val && bowcharge < 40) {
			isChargingBow = true;
			Camera->FieldOfView -= 1;
			bowcharge++;
		}
	}
}

void AMouseCharacter::ShootBow(const FInputActionValue& InputValue) {
	bool val = InputValue.Get<bool>();
	if (ArrowCountOfAmmunitionForBow > 0) {
		float VelocityMultiplier = (float)bowcharge / 120;
		AActor* spawnedArrow = GetWorld()->SpawnActor<AActor>(Arrow, GetActorLocation() + GetController()->GetControlRotation().Vector(), GetController()->GetControlRotation());
		Cast<AArrow>(spawnedArrow)->arrowVelocity = FVector(VelocityMultiplier, 0, VelocityMultiplier);
		ArrowCountOfAmmunitionForBow -= 1;
		bowcharge = 0;
		isChargingBow = false;
		Camera->FieldOfView = 90;
	}
}    

void AMouseCharacter::MovePlayer(float tickdelta) {
	appplyGravity(10);
	if (isSprinting) {
		this->GetCharacterMovement()->MaxWalkSpeed = 750;
	}
	else {
		this->GetCharacterMovement()->MaxWalkSpeed = 500;
	}
	if (FMath::Abs(playerVelocity.X) >= maxHorizontalVelocity) {
		this->playerVelocity.X = this->playerVelocity.X > 0 ? maxHorizontalVelocity : maxHorizontalVelocity * -1;
	}
	if (FMath::Abs(playerVelocity.Y) >= maxHorizontalVelocity) {
		this->playerVelocity.Y = this->playerVelocity.Y > 0 ? maxHorizontalVelocity : maxHorizontalVelocity * -1;
	}
	AddMovementInput(playerVelocity.X * UKismetMathLibrary::GetForwardVector(Controller->GetControlRotation()));
	AddMovementInput(playerVelocity.Y * UKismetMathLibrary::GetRightVector(Controller->GetControlRotation()));
}

void AMouseCharacter::appplyGravity(float strenght) {
	playerVelocity.Z -= strenght;
}

void AMouseCharacter::ManageStamina() {
	if (isSprinting) {
		Stamina -= 5;
	}
	else if (Stamina < 1000) {
		Stamina += 1.5;
	}
	if (Stamina <= 0) {
		exhausted = true;
	}
	if (Stamina >= 900) {
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
	Stamina = 1000;
	exhausted = false;
	bowcharge = 0;
}

void AMouseCharacter::CamInCave()
{
	Camera->SetRelativeLocation(FVector::ZeroVector);
}

void AMouseCharacter::CamOutOfCave()
{
	Camera->SetRelativeLocation(FVector(-1000.f, 0.f, 200.f));
}

void AMouseCharacter::onWin_Implementation()
{
	WonGame = true;
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

void AMouseCharacter::HurtPlayer()
{
	if (invulTicks <= 0) {
		if (lives > 1) {
			lives -= 1;
			invulTicks = 100;
		}
		else {
			onDeath();
		}
	}
}

