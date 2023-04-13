// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "Components/SphereComponent.h"
#include "MouseCharacter.h"

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

	arrowVelocity = GetActorRotation().Vector() * FVector(0.5, 0.5, 0.5);
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
	appplyGravity(0.005);
	ArrowMesh->SetRelativeLocation(ArrowMesh->GetRelativeLocation() + arrowVelocity * 5000 * tickdelta);
	arrowVelocity *= FVector(0.95, 0.95, 1);
}

void AArrow::appplyGravity(float strenght) {
	arrowVelocity.Z -= strenght;
}
