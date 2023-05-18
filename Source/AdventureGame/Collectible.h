// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Collectible.generated.h"

UCLASS()
class ADVENTUREGAME_API ACollectible : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollectible();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector velocity;
	bool onGround;

	UPROPERTY(BlueprintReadWrite, Category = "Collectible")
	int ticksPassed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
		class UStaticMeshComponent* CollectibleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
		class USphereComponent* DetectionSphere{ nullptr };



	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void despawn();

		virtual void Collected(AActor* OtherActor);
		virtual void onTick(float tickdelta);

		void RotateTick(float tickdelta);
		void HoverTick(float tickdelta);

};
