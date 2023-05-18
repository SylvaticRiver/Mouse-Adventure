// Fill out your copyright notice in the Description page of Project Settings.


#include "Berry.h"
#include "MouseCharacter.h"


void ABerry::onTick(float tickdelta)
{
	RotateTick(tickdelta);
	HoverTick(tickdelta);
}

void ABerry::Collected(AActor* OtherActor)
{
	Cast<AMouseCharacter>(OtherActor)->Stamina += 1000;
	despawn();
}
