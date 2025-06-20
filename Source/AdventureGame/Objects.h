// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Objects.generated.h"

UCLASS()
class ADVENTUREGAME_API AObjects : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjects();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool isDestroyed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
		class UStaticMeshComponent* ObjectMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
		int DestructionDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
		bool requiresSpecialArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
		class UBoxComponent* CollisionBox{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
		TSubclassOf<class ACollectible> LootItem;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	void destroyAndDropLoot();
};
