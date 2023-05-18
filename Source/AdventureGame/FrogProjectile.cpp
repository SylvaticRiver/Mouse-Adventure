// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogProjectile.h"
#include "MouseCharacter.h"
#include "Components/SphereComponent.h"
#include "Arrow.h"


// Sets default values
AFrogProjectile::AFrogProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("¨ProjectileMesh"));
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBox"));
	SetRootComponent(DetectionSphere);
	DetectionSphere->InitSphereRadius(50);
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AFrogProjectile::OnOverlap);
	ProjectileMesh->SetupAttachment(GetRootComponent());

	ticksAlive = 0;
	frogLocation = GetActorLocation();
	targetLocation = FVector(0, 0, 0);
}

// Called when the game starts or when spawned
void AFrogProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFrogProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Travel(DeltaTime);
	ticksAlive++;
	if (ticksAlive >= 300) {
		despawn();
	}
}

void AFrogProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMouseCharacter>()) {
		Cast<AMouseCharacter>(OtherActor)->HurtPlayer();
		despawn();
	}
	if (OtherActor->IsA<AArrow>()) {
		Cast<AArrow>(OtherActor)->despawn();
		despawn();
	}
}

void AFrogProjectile::despawn()
{
	this->SetActorEnableCollision(false);
	this->SetActorHiddenInGame(true);
	this->Destroy();
}

void AFrogProjectile::Travel(float tickDelta)
{
	FVector intermediateSteps = (targetLocation - frogLocation) * tickDelta;
	SetActorLocation(GetActorLocation() + intermediateSteps);
}
