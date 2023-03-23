// Fill out your copyright notice in the Description page of Project Settings.


#include "Berry.h"
#include "MouseCharacter.h"

void ABerry::Collected(AActor* OtherActor)
{
	if (OtherActor->IsA<AMouseCharacter>()) {
		Cast<AMouseCharacter>(OtherActor)->SetActorLocation(FVector(100, 100, 100));
		despawn();
	}
}
