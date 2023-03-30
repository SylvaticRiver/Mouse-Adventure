// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectible.h"
#include "Components/SphereComponent.h"
#include "MouseCharacter.h"

// Sets default values
ACollectible::ACollectible()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollectibleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectibleMesh"));
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBox"));
	SetRootComponent(DetectionSphere);
	DetectionSphere->InitSphereRadius(50);
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACollectible::OnOverlap);
	CollectibleMesh->SetupAttachment(GetRootComponent());

	ticksPassed = 0;
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

	if (ticksPassed <= 360) {
		ticksPassed++;
	}
	else {
		ticksPassed = 0;
	}
	onTick(DeltaTime);
}

// Called to bind functionality to input
void ACollectible::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACollectible::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMouseCharacter>()) {
		Collected(OtherActor);
	}
}

void ACollectible::despawn()
{
	this->SetActorEnableCollision(false);
	this->SetActorHiddenInGame(true);
	this->Destroy();
}

void ACollectible::Collected(AActor* OtherActor)
{

}

void ACollectible::onTick(float tickdelta)
{

}

void ACollectible::RotateTick(float tickdelta)
{
	CollectibleMesh->SetRelativeRotation(FRotator3d(CollectibleMesh->GetRelativeRotation().Pitch, -ticksPassed * 4, CollectibleMesh->GetRelativeRotation().Roll));
}

void ACollectible::HoverTick(float tickdelta)
{
	float HoverHeight = FMath::Cos(ticksPassed / 10);
	CollectibleMesh->SetRelativeLocation(FVector(CollectibleMesh->GetRelativeLocation().X, CollectibleMesh->GetRelativeLocation().Y, CollectibleMesh->GetRelativeLocation().Z + HoverHeight));
}

