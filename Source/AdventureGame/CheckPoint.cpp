// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint.h"
#include "Components/BoxComponent.h"
#include "MouseCharacter.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(DetectionBox);
	DetectionBox->InitBoxExtent(FVector(50, 50, 50));
	DetectionBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnOverlap);
}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACheckPoint::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMouseCharacter>()) {
		Cast<AMouseCharacter>(OtherActor)->lastCheckpointPos = this->GetActorLocation();
	}
}

