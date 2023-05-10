// Fill out your copyright notice in the Description page of Project Settings.


#include "Nut.h"
#include "MouseCharacter.h"

void ANut::onTick(float tickdelta)
{
	RotateTick(tickdelta);
	HoverTick(tickdelta);
}

void ANut::Collected(AActor* OtherActor)
{
	if (OtherActor->IsA<AMouseCharacter>()) {
		Cast<AMouseCharacter>(OtherActor)->lives = 5;
		despawn();
	}
}
