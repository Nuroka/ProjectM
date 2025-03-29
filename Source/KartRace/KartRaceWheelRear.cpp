// Copyright Epic Games, Inc. All Rights Reserved.

#include "KartRaceWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UKartRaceWheelRear::UKartRaceWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}