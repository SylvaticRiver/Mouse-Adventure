// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FrogEnemy.generated.h"

UCLASS()
class ADVENTUREGAME_API AFrogEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFrogEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class AMouseCharacter* Target;
	FVector startPos;
	int jumpApexTicks;
	bool isAttacking;
	int attackCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frog")
		class USkeletalMeshComponent* FrogMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frog")
		class USphereComponent* DetectionSphere{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frog")
		TSubclassOf<class AFrogProjectile> Projectile;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	float getDistanceToTarget(AMouseCharacter* target);
	void jumpUp(float tickDelta);
	void ShootAt(AMouseCharacter* target);
	void Descend(float tickDelta);
	void attack(AMouseCharacter* target, float tickDelta);
};
