// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoardwalkPedestrianLine.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

ABoardwalkPedestrianLine::ABoardwalkPedestrianLine()
{
	PrimaryActorTick.bCanEverTick = false;

	Blocker = CreateDefaultSubobject<UBoxComponent>(TEXT("Blocker"));
	SetRootComponent(Blocker);
	Blocker->SetBoxExtent(FVector(220.0f, 70.0f, 155.0f));
	Blocker->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Blocker->SetCollisionObjectType(ECC_WorldDynamic);
	Blocker->SetCollisionResponseToAllChannels(ECR_Block);
	Blocker->SetGenerateOverlapEvents(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	for (int32 Index = 0; Index < 4; ++Index)
	{
		const float XOffset = -150.0f + Index * 100.0f;
		const FString BodyName = FString::Printf(TEXT("PersonBody%d"), Index);
		const FString HeadName = FString::Printf(TEXT("PersonHead%d"), Index);

		UStaticMeshComponent* Body = CreateDefaultSubobject<UStaticMeshComponent>(FName(*BodyName));
		Body->SetupAttachment(Blocker);
		if (CubeAsset.Succeeded()) Body->SetStaticMesh(CubeAsset.Object);
		Body->SetRelativeLocation(FVector(XOffset, -55.0f, -25.0f));
		Body->SetRelativeScale3D(FVector(0.38f, 0.42f, 1.15f));
		Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CrowdParts.Add(Body);

		UStaticMeshComponent* Head = CreateDefaultSubobject<UStaticMeshComponent>(FName(*HeadName));
		Head->SetupAttachment(Blocker);
		if (SphereAsset.Succeeded()) Head->SetStaticMesh(SphereAsset.Object);
		Head->SetRelativeLocation(FVector(XOffset, -55.0f, 65.0f));
		Head->SetRelativeScale3D(FVector(0.38f));
		Head->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CrowdParts.Add(Head);
	}
}

void ABoardwalkPedestrianLine::BeginPlay()
{
	Super::BeginPlay();
	DynamicMaterials.Reset();
	for (UStaticMeshComponent* Part : CrowdParts)
	{
		DynamicMaterials.Add(Part ? Part->CreateAndSetMaterialInstanceDynamic(0) : nullptr);
	}
	ApplyWorldState();
}

void ABoardwalkPedestrianLine::SetDirectorCutActive_Implementation(bool bNewIsDirectorCut)
{
	bIsDirectorCut = bNewIsDirectorCut;
	ApplyWorldState();
}

void ABoardwalkPedestrianLine::ApplyWorldState()
{
	const bool bCrowdActive = !bIsDirectorCut;
	Blocker->SetCollisionEnabled(bCrowdActive ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	SetActorHiddenInGame(!bCrowdActive);

	const FLinearColor Clothes[] = {
		FLinearColor(0.20f, 0.44f, 0.88f),
		FLinearColor(0.92f, 0.28f, 0.16f),
		FLinearColor(0.15f, 0.62f, 0.38f),
		FLinearColor(0.92f, 0.66f, 0.16f)
	};
	const FLinearColor Skin(0.86f, 0.61f, 0.43f);
	for (int32 Index = 0; Index < DynamicMaterials.Num(); ++Index)
	{
		if (UMaterialInstanceDynamic* Material = DynamicMaterials[Index])
		{
			const FLinearColor Color = Index % 2 == 0 ? Clothes[Index / 2] : Skin;
			Material->SetVectorParameterValue(TEXT("BaseColor"), Color);
		}
	}
}
