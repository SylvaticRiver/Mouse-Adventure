// Fill out your copyright notice in the Description page of Project Settings.


#include "GnomeCollectible.h"
#include "MouseCharacter.h"

void AGnomeCollectible::onTick(float tickdelta)
{
	RotateTick(tickdelta);
	HoverTick(tickdelta);
}

void AGnomeCollectible::Collected(AActor* OtherActor)
{
	Cast<AMouseCharacter>(OtherActor)->gnomePiecesCollected += 1;
	UE_LOG(LogTemp, Warning, TEXT("GnomePieces collected = %d"), Cast<AMouseCharacter>(OtherActor)->gnomePiecesCollected);
	despawn();
}