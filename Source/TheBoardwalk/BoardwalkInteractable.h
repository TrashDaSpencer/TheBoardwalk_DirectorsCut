// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BoardwalkInteractable.generated.h"

UINTERFACE(BlueprintType)
class THEBOARDWALK_API UBoardwalkInteractable : public UInterface
{
	GENERATED_BODY()
};

/** Implement on actors the player can use or kick. */
class THEBOARDWALK_API IBoardwalkInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Boardwalk|Interaction")
	void Interact(AActor* InteractingActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Boardwalk|Combat")
	void ReceivePlayerKick(AActor* AttackingActor);
};
