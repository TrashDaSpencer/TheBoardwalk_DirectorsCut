// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BoardwalkWorldVariant.generated.h"

/** Implement on level actors whose appearance or behavior changes between the two cuts. */
UINTERFACE(BlueprintType)
class THEBOARDWALK_API UBoardwalkWorldVariant : public UInterface
{
	GENERATED_BODY()
};

class THEBOARDWALK_API IBoardwalkWorldVariant
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Boardwalk|World")
	void SetDirectorCutActive(bool bIsDirectorCut);
};
