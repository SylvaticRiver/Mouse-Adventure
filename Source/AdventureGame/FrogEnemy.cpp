// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogEnemy.h"
#include "Components/SphereComponent.h"
#include "MouseCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "FrogProjectile.h"

// Sets default values
AFrogEnemy::AFrogEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FrogMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FrogMesh"));
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionBox"));
	SetRootComponent(DetectionSphere);
	DetectionSphere->InitSphereRadius(1000);
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AFrogEnemy::OnOverlap);
	FrogMesh->SetupAttachment(GetRootComponent());

	jumpApexTicks = 0;
	attackCooldown = 0;
	startPos = GetActorLocation();
	isAttacking = false;
}

// Called when the game starts or when spawned
void AFrogEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFrogEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target != NULL) {
		UE_LOG(LogTemp, Warning, TEXT("Player!"));
		if (GetDistanceTo(Target) >= DetectionSphere->GetScaledSphereRadius() + 300) {
			Target = NULL;
		}
		else {
			if (attackCooldown <= 0) {
				attack(Target, DeltaTime);
			}
		}
	}
	if (attackCooldown > 0) {
		attackCooldown--;
	}
	if (Target == NULL && jumpApexTicks != 0) {
		Descend(DeltaTime);
	}
}

void AFrogEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMouseCharacter>()) {
		Target = Cast<AMouseCharacter>(OtherActor);
	}
}

float AFrogEnemy::getDistanceToTarget(AMouseCharacter* target)
{
	FVector playerPos = target->GetActorLocation();
	FVector FrogPos = GetActorLocation();

	return FVector::Dist(FrogPos, playerPos);
}

void AFrogEnemy::jumpUp(float tickDelta)
{
	for (int i = 0; i < 40; i++) {
		SetActorLocation(GetActorLocation() + FVector(0, 0, 0.5 * i));
	}
	jumpApexTicks = 1;
}

void AFrogEnemy::ShootAt(AMouseCharacter* target)
{
	if (!isAttacking) {
		AActor* spawnedProjectile = GetWorld()->SpawnActor<AActor>(Projectile, GetActorLocation() + GetActorRotation().Vector(), GetActorRotation());
		if (spawnedProjectile != NULL && Projectile != NULL) {
			Cast<AFrogProjectile>(spawnedProjectile)->targetLocation = target->GetActorLocation();
			Cast<AFrogProjectile>(spawnedProjectile)->frogLocation = GetActorLocation();
		}
		isAttacking = true;
	}
}

void AFrogEnemy::Descend(float tickDelta)
{
	for (int i = 0; i < 40; i++) {
		SetActorLocation(GetActorLocation() + FVector(0, 0, -0.5 * i));
	}
	jumpApexTicks = 0;
	attackCooldown = 100;
	isAttacking = false;
}

void AFrogEnemy::attack(AMouseCharacter* target, float tickDelta)
{
	if (jumpApexTicks == 0) {
		jumpUp(tickDelta);
	}
	if (jumpApexTicks > 0) {
		jumpApexTicks++;
		if (jumpApexTicks == 20) {
			ShootAt(target);
		}
		if (jumpApexTicks >= 40) {
			Descend(tickDelta);
		}
	}
}

