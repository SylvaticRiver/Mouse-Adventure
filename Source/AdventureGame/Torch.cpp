// Fill out your copyright notice in the Description page of Project Settings.


#include "Torch.h"
#include "Components/SphereComponent.h"
#include "Arrow.h"
#include "MouseCharacter.h"

// Sets default values
ATorch::ATorch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TorchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrogMesh"));
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionBox"));
	SetRootComponent(DetectionSphere);
	DetectionSphere->InitSphereRadius(50);
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATorch::OnOverlap);
	TorchMesh->SetupAttachment(GetRootComponent());

	isIgnited = false;
}

// Called when the game starts or when spawned
void ATorch::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATorch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATorch::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AArrow>() && isIgnited) {
		Cast<AArrow>(OtherActor)->isBurning = true;
	}
	if (OtherActor->IsA<AMouseCharacter>()) {
		isIgnited = true;
	}
}

