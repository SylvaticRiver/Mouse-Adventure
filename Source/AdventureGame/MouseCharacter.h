// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MouseCharacter.generated.h"

UCLASS()
class ADVENTUREGAME_API AMouseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMouseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool isSprinting;
	FVector playerVelocity;
	FVector prevVelocity;
	float maxHorizontalVelocity;
	float terminalVelocity;
	bool isJumping;
	bool isCrouching;
	bool exhausted;
	int bowcharge;
	FVector lastCheckpointPos;
	int gnomePiecesCollected;

	UPROPERTY(BlueprintReadWrite, Category = "Mouse")
	int ArrowCountOfAmmunitionForBow;

	UPROPERTY(BlueprintReadWrite, Category = "Mouse")
	int lives;

	UPROPERTY(BlueprintReadWrite, Category = "Mouse")
	int Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse")
		class USkeletalMeshComponent* MouseMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse")
		class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse")
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse")
		class UInputMappingContext* InputMapping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse")
		class UInputAction* IA_MoveStraight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse")
		class UInputAction* IA_MoveSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse")
		class UInputAction* IA_RotateX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse")
		class UInputAction* IA_RotateY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse")
		class UInputAction* IA_Sprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse")
		class UInputAction* IA_Jump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse")
		class UInputAction* IA_Crouch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse")
		class UInputAction* IA_Bow;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse")
		TSubclassOf<class AArrow> Arrow;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			bool hurt;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			int invulTicks;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			bool isChargingBow;

public:
	UFUNCTION(BLueprintCallable)
		void MoveStraight(const FInputActionValue& InputValue);
	UFUNCTION(BLueprintCallable)
		void MoveSide(const FInputActionValue& InputValue);
	UFUNCTION(BLueprintCallable)
		void Sprint(const FInputActionValue& InputValue);
	UFUNCTION(BLueprintCallable)
		void StopSprint(const FInputActionValue& InputValue);
	UFUNCTION(BLueprintCallable)
		void JumpUp(const FInputActionValue& InputValue);
	UFUNCTION(BLueprintCallable)
		void JumpUpComplete(const FInputActionValue& InputValue);
	UFUNCTION(BLueprintCallable)
		void CrouchDown(const FInputActionValue& InputValue);
	UFUNCTION(BLueprintCallable)
		void CrouchComplete(const FInputActionValue& InputValue);
	UFUNCTION(BLueprintCallable)
		void RotateX(const FInputActionValue& InputValue);
	UFUNCTION(BLueprintCallable)
		void RotateY(const FInputActionValue& InputValue);
	UFUNCTION(BLueprintCallable)
		void ChargeBow(const FInputActionValue& InputValue);
	UFUNCTION(BLueprintCallable)
		void ShootBow(const FInputActionValue& InputValue);
	UFUNCTION(BLueprintCallable)
		void CattailSling(const FInputActionValue& InputValue);


	void MovePlayer(float tickdelta);
	void appplyGravity(float strenght);
	bool isOnGround();
	void ManageStamina();
	void onDeath();

	UFUNCTION(BluePrintCallable)
		float GetXVelocity();
	UFUNCTION(BluePrintCallable)
		float GetYVelocity();
	UFUNCTION(BluePrintCallable)
		float GetZVelocity();

	UFUNCTION(BluePrintCallable)
		void HurtPlayer();
};
