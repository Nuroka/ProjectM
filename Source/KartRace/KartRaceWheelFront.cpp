// Copyright Epic Games, Inc. All Rights Reserved.

#include "KartRaceWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UKartRaceWheelFront::UKartRaceWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}