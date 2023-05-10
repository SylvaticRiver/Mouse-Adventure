// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "Components/SphereComponent.h"
#include "MouseCharacter.h"
#include "Objects.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBox"));
	SetRootComponent(DetectionSphere);
	DetectionSphere->InitSphereRadius(50);
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnOverlap);
	ArrowMesh->SetupAttachment(GetRootComponent());

	ticksAlive = 0;
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	travel(DeltaTime);
	ticksAlive++;
	if (ticksAlive >= 60) {
		despawn();
	}
}

void AArrow::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AArrow::despawn()
{
	this->SetActorEnableCollision(false);
	this->SetActorHiddenInGame(true);
	this->Destroy();
}

void AArrow::travel(float tickdelta)
{
	appplyGravity(0.01);
	ArrowMesh->SetRelativeLocation(ArrowMesh->GetRelativeLocation() + arrowVelocity * 5000 * tickdelta);
	/*SetActorRelativeLocation(GetActorLocation() + arrowVelocity * 5000 * tickdelta);*/
}

void AArrow::appplyGravity(float strenght) {
	arrowVelocity.Z -= strenght;
}
