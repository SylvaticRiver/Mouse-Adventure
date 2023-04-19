// Fill out your copyright notice in the Description page of Project Settings.


#include "Cattail.h"
#include "MouseCharacter.h"

void ACattail::onTick(float tickdelta)
{

}

void ACattail::Collected(AActor* OtherActor)
{
	Cast<AMouseCharacter>(OtherActor)->SetActorLocation(FVector(100, 100, 100));
	UE_LOG(LogTemp, Warning, TEXT("FUCK YOU"));
}