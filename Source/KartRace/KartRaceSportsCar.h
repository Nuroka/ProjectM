// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "KartRacePawn.h"
#include "KartRaceSportsCar.generated.h"

/**
 *  Sports car wheeled vehicle implementation
 */
UCLASS(abstract)
class KARTRACE_API AKartRaceSportsCar : public AKartRacePawn
{
	GENERATED_BODY()
	
public:

	AKartRaceSportsCar();
};
