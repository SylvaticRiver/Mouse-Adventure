// Fill out your copyright notice in the Description page of Project Settings.


#include "Cattail.h"
#include "Components/SphereComponent.h"
#include "MouseCharacter.h"

// Sets default values
ACattail::ACattail()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CattailMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CattailMesh"));
	SetRootComponent(CattailMesh);
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBox"));
	DetectionSphere->InitSphereRadius(100);
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACattail::OnOverlap);
	this->Rotator = GetActorRotation();
}

// Called when the game starts or when spawned
void ACattail::BeginPlay()
{
	Super::BeginPlay();
	
	this->Rotator = GetActorRotation();
	CattailMesh->SetRelativeRotation(this->Rotator);
}

// Called every frame
void ACattail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACattail::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMouseCharacter>()) {
		UE_LOG(LogTemp, Warning, TEXT("Collide"));
		SetActorRotation(FRotator(0, 0, 0));
		this->Rotator = GetActorRotation();
	}
}

