// Fill out your copyright notice in the Description page of Project Settings.


#include "Mushroom.h"
#include "MouseCharacter.h"

void AMushroom::Collected(AActor* OtherActor)
{
	Cast<AMouseCharacter>(OtherActor)->MouseMesh->SetRelativeLocation(FVector(Cast<AMouseCharacter>(OtherActor)->MouseMesh->GetRelativeLocation().X, Cast<AMouseCharacter>(OtherActor)->MouseMesh->GetRelativeLocation().Y, Cast<AMouseCharacter>(OtherActor)->MouseMesh->GetRelativeLocation().Z + 120));
	float z = Cast<AMouseCharacter>(OtherActor)->playerVelocity.Z;	
	Cast<AMouseCharacter>(OtherActor)->playerVelocity.Z += 100;
}
