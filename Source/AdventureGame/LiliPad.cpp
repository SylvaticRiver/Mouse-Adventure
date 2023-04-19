// Fill out your copyright notice in the Description page of Project Settings.


#include "LiliPad.h"
#include "Components/BoxComponent.h"
#include "MouseCharacter.h"

// Sets default values
ALiliPad::ALiliPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LilyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	DetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(DetectionBox);
	DetectionBox->InitBoxExtent(FVector(50, 50, 50));
	DetectionBox->OnComponentBeginOverlap.AddDynamic(this, &ALiliPad::OnOverlap);
	LilyMesh->SetupAttachment(GetRootComponent());

	stabilityTicks = 200;
	isSinking = false;	
	submergedPosition = GetActorLocation() - FVector(0, 0, 30);
	originalPosition = DetectionBox->GetRelativeLocation();
}

// Called when the game starts or when spawned
void ALiliPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALiliPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	startShaking(DeltaTime);
	if (isSinking) {
		startSinking(DeltaTime);
	}
}

void ALiliPad::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMouseCharacter>()) {
		isUnstable = true;
		/*UE_LOG(LogTemp, Warning, TEXT("HELP I'M SINKING"));*/
	}
}


void ALiliPad::startShaking(float tickDelta)
{
	if (isUnstable && stabilityTicks > 0) {
		stabilityTicks -= 1 * tickDelta;
		/*UE_LOG(LogTemp, Warning, TEXT("stability = %d"), stabilityTicks);*/
	}
	if (stabilityTicks == 0) {
		isSinking = true;
		submergeTimer = 200;
		stabilityTicks = -1;
	}
}

void ALiliPad::startSinking(float tickDelta)
{
	if (submergeTimer > 0) {
		/*>.<*/
		removeFromLevel(true);
		submergeTimer -= 1 * tickDelta;
		/*UE_LOG(LogTemp, Warning, TEXT("submerged = %d"), submergeTimer);*/
	}
	else if (submergeTimer <= 0) {
		removeFromLevel(false);
		submergeTimer = 0;
		isUnstable = false;
		stabilityTicks = 200;
		isSinking = false;
	}
}

void ALiliPad::removeFromLevel(bool isRemoved)
{
	this->SetActorEnableCollision(!isRemoved);
	this->SetActorHiddenInGame(isRemoved);
}
