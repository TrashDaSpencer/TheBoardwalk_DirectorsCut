// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoardwalkGameMode.h"
#include "BoardwalkCharacter.h"

ABoardwalkGameMode::ABoardwalkGameMode()
{
	DefaultPawnClass = ABoardwalkCharacter::StaticClass();
}
