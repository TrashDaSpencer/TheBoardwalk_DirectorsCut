// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardwalkWorldVariant.h"
#include "BoardwalkBuilding.generated.h"

class UMaterialInstanceDynamic;
class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class THEBOARDWALK_API ABoardwalkBuilding : public AActor, public IBoardwalkWorldVariant
{
	GENERATED_BODY()

public:
	ABoardwalkBuilding();
	virtual void SetDirectorCutActive_Implementation(bool bIsDirectorCut) override;
	void SetBuildingStyle(int32 StyleIndex, bool bIsDirectorCut);

protected:
	virtual void BeginPlay() override;

private:
	void ApplyPalette();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TArray<TObjectPtr<UStaticMeshComponent>> VisualParts;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicMaterials;

	int32 StyleIndex = 0;
	bool bIsDirectorCut = false;
};
