// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardwalkWorldVariant.h"
#include "BoardwalkPedestrianLine.generated.h"

class UBoxComponent;
class UMaterialInstanceDynamic;
class UStaticMeshComponent;

UCLASS()
class THEBOARDWALK_API ABoardwalkPedestrianLine : public AActor, public IBoardwalkWorldVariant
{
	GENERATED_BODY()

public:
	ABoardwalkPedestrianLine();
	virtual void SetDirectorCutActive_Implementation(bool bIsDirectorCut) override;

protected:
	virtual void BeginPlay() override;

private:
	void ApplyWorldState();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> Blocker;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TArray<TObjectPtr<UStaticMeshComponent>> CrowdParts;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicMaterials;

	bool bIsDirectorCut = false;
};
