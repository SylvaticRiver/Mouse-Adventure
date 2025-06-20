// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects.h"
#include "Components/BoxComponent.h"
#include "Arrow.h"
#include "Collectible.h"

// Sets default values
AObjects::AObjects()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectibleMesh"));
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->InitBoxExtent(FVector(50, 50, 50));
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AObjects::OnOverlap);
	ObjectMesh->SetupAttachment(GetRootComponent());

	isDestroyed = false;
}

// Called when the game starts or when spawned
void AObjects::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isDestroyed) {
		if (DestructionDelay <= 0) {
			destroyAndDropLoot();
		}
		else {
			DestructionDelay--;
		}
	}
}

void AObjects::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AArrow>()) {
		if (!requiresSpecialArrow || (requiresSpecialArrow && Cast<AArrow>(OtherActor)->isBurning)) {
			isDestroyed = true;
		}
	}
}

void AObjects::destroyAndDropLoot()
{
	this->SetActorEnableCollision(false);
	this->SetActorHiddenInGame(true);
	if (LootItem != NULL) {
		AActor* spawnedLoot = GetWorld()->SpawnActor<AActor>(LootItem, GetActorLocation() - (CollisionBox->GetScaledBoxExtent() / 2), GetActorRotation());
	}
	this->Destroy();
}

