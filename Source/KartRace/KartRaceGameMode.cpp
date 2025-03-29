// Copyright Epic Games, Inc. All Rights Reserved.

#include "KartRaceGameMode.h"
#include "KartRacePlayerController.h"

AKartRaceGameMode::AKartRaceGameMode()
{
	PlayerControllerClass = AKartRacePlayerController::StaticClass();
}
