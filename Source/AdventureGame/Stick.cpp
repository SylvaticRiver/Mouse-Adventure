// Fill out your copyright notice in the Description page of Project Settings.


#include "Stick.h"
#include "MouseCharacter.h"

void AStick::onTick(float tickdelta)
{
	RotateTick(tickdelta);
	HoverTick(tickdelta);
}

void AStick::Collected(AActor* OtherActor)
{
	Cast<AMouseCharacter>(OtherActor)->ArrowCountOfAmmunitionForBow = 10;
	despawn();
}
