// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainPlayer.generated.h"

UCLASS()
class ADVENTUREGAME_API AMainPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainPlayer();

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
	float terminalVelocity;
	bool isJumping;
	bool isCrouching;

	int lives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse")
		class UStaticMeshComponent* MouseMesh;
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
		class UInputAction* IA_Sprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse")
		class UInputAction* IA_Jump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse")
		class UInputAction* IA_Crouch;

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
		void Jump(const FInputActionValue& InputValue);
	UFUNCTION(BLueprintCallable)
		void Crouch(const FInputActionValue& InputValue);
	UFUNCTION(BLueprintCallable)
		void CrouchComplete(const FInputActionValue& InputValue);

	void MovePlayer();
	void appplyGravity(float strenght);
	bool isOnGround();
};
