// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cattail.generated.h"

UCLASS()
class ADVENTUREGAME_API ACattail : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACattail();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FRotator Rotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cattail")
		class UStaticMeshComponent* CattailMesh;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cattail")
		class USphereComponent* DetectionSphere{ nullptr };

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	void LerpRotation();
};
