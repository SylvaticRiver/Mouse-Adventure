// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectible.h"
#include "Components/BoxComponent.h"
#include "MouseCharacter.h"

// Sets default values
ACollectible::ACollectible()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollectibleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectibleMesh"));
	DetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(DetectionBox);
	DetectionBox->InitBoxExtent(FVector(50, 50, 50));
	DetectionBox->OnComponentBeginOverlap.AddDynamic(this, &ACollectible::OnOverlap);
	CollectibleMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ACollectible::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

// Called to bind functionality to input
void ACollectible::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACollectible::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Collected(OtherActor);
}

void ACollectible::despawn()
{
	this->SetActorEnableCollision(false);
	this->SetActorHiddenInGame(true);
	this->Destroy();
}

void ACollectible::Collected(AActor* OtherActor)
{
	if (OtherActor->IsA<AMouseCharacter>()) {
		despawn();
		UE_LOG(LogTemp, Warning, TEXT("COLLECTED"));
	}
}

