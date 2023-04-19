// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LiliPad.generated.h"

UCLASS()
class ADVENTUREGAME_API ALiliPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALiliPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int stabilityTicks;
	int submergeTimer;
	bool isUnstable;
	bool isSinking;
	FVector submergedPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lilypad")
		class UStaticMeshComponent* LilyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lilypad")
		class UBoxComponent* DetectionBox{ nullptr };

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	void startShaking(float tickDelta);
	void startSinking(float tickDelta);
};
