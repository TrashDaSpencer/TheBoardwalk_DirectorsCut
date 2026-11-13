// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoardwalkBuilding.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

ABoardwalkBuilding::ABoardwalkBuilding()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (!CubeAsset.Succeeded())
	{
		return;
	}

	auto AddPart = [this](const TCHAR* Name, const FVector& Location, const FVector& Scale)
	{
		UStaticMeshComponent* Part = CreateDefaultSubobject<UStaticMeshComponent>(FName(Name));
		Part->SetupAttachment(SceneRoot);
		Part->SetStaticMesh(CubeAsset.Object);
		Part->SetRelativeLocation(Location);
		Part->SetRelativeScale3D(Scale);
		Part->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		VisualParts.Add(Part);
	};

	AddPart(TEXT("Facade"), FVector(0.0f, 0.0f, 110.0f), FVector(7.0f, 0.6f, 4.0f));
	AddPart(TEXT("Roof"), FVector(0.0f, 0.0f, 340.0f), FVector(7.6f, 1.0f, 0.55f));
	AddPart(TEXT("Awning"), FVector(0.0f, -35.0f, 225.0f), FVector(5.4f, 0.6f, 0.38f));
	AddPart(TEXT("Door"), FVector(-245.0f, -32.0f, 2.0f), FVector(0.8f, 0.5f, 2.0f));
	AddPart(TEXT("WindowLeft"), FVector(-95.0f, -34.0f, 145.0f), FVector(0.75f, 0.25f, 0.8f));
	AddPart(TEXT("WindowRight"), FVector(145.0f, -34.0f, 145.0f), FVector(0.75f, 0.25f, 0.8f));
}

void ABoardwalkBuilding::BeginPlay()
{
	Super::BeginPlay();
	DynamicMaterials.Reset();
	for (UStaticMeshComponent* Part : VisualParts)
	{
		DynamicMaterials.Add(Part ? Part->CreateAndSetMaterialInstanceDynamic(0) : nullptr);
	}
	ApplyPalette();
}

void ABoardwalkBuilding::SetBuildingStyle(int32 NewStyleIndex, bool bNewIsDirectorCut)
{
	StyleIndex = NewStyleIndex % 2;
	bIsDirectorCut = bNewIsDirectorCut;
	ApplyPalette();
}

void ABoardwalkBuilding::SetDirectorCutActive_Implementation(bool bNewIsDirectorCut)
{
	bIsDirectorCut = bNewIsDirectorCut;
	ApplyPalette();
}

void ABoardwalkBuilding::ApplyPalette()
{
	const FLinearColor DayFacades[] = { FLinearColor(0.95f, 0.55f, 0.26f), FLinearColor(0.25f, 0.68f, 0.72f) };
	const FLinearColor NightFacades[] = { FLinearColor(0.12f, 0.09f, 0.22f), FLinearColor(0.07f, 0.15f, 0.25f) };
	const FLinearColor DayRoof(0.92f, 0.82f, 0.56f);
	const FLinearColor NightRoof(0.055f, 0.045f, 0.10f);
	const FLinearColor DayAwning = StyleIndex == 0 ? FLinearColor(0.88f, 0.18f, 0.12f) : FLinearColor(0.96f, 0.74f, 0.18f);
	const FLinearColor NightAwning(0.20f, 0.10f, 0.25f);
	const FLinearColor DayDoor(0.30f, 0.14f, 0.08f);
	const FLinearColor NightDoor(0.035f, 0.025f, 0.06f);
	const FLinearColor DayWindow(0.30f, 0.78f, 0.95f);
	const FLinearColor NightWindow(1.0f, 0.55f, 0.16f);

	for (int32 Index = 0; Index < DynamicMaterials.Num(); ++Index)
	{
		UMaterialInstanceDynamic* Material = DynamicMaterials[Index];
		if (!Material)
		{
			continue;
		}

		FLinearColor Color;
		if (Index == 0)
		{
			Color = bIsDirectorCut ? NightFacades[StyleIndex] : DayFacades[StyleIndex];
		}
		else if (Index == 1)
		{
			Color = bIsDirectorCut ? NightRoof : DayRoof;
		}
		else if (Index == 2)
		{
			Color = bIsDirectorCut ? NightAwning : DayAwning;
		}
		else if (Index == 3)
		{
			Color = bIsDirectorCut ? NightDoor : DayDoor;
		}
		else
		{
			Color = bIsDirectorCut ? NightWindow : DayWindow;
		}

		Material->SetVectorParameterValue(TEXT("BaseColor"), Color);
		if (Index >= 4)
		{
			Material->SetVectorParameterValue(TEXT("EmissiveColor"), bIsDirectorCut ? NightWindow : FLinearColor::Black);
		}
	}
}
