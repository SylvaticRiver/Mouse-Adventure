// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathBarrier.h"
#include "Components/BoxComponent.h"
#include "MouseCharacter.h"

// Sets default values
ADeathBarrier::ADeathBarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(DetectionBox);
	DetectionBox->InitBoxExtent(FVector(250, 250, 250));
	DetectionBox->OnComponentBeginOverlap.AddDynamic(this, &ADeathBarrier::OnOverlap);
}

// Called when the game starts or when spawned
void ADeathBarrier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADeathBarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeathBarrier::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMouseCharacter>()) {
		Cast<AMouseCharacter>(OtherActor)->onDeath();
	}
}

