// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectible.h"
#include "Berry.generated.h"

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API ABerry : public ACollectible
{
	GENERATED_BODY()

	virtual void Collected(AActor* OtherActor);
};
