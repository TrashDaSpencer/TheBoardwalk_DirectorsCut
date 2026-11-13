// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoardwalkRoamingNPC.h"

#include "Components/BillboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Texture2D.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

ABoardwalkRoamingNPC::ABoardwalkRoamingNPC()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(34.0f, 92.0f);
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 1.0f, 0.0f));
	GetCharacterMovement()->SetPlaneConstraintOrigin(FVector::ZeroVector);
	GetCharacterMovement()->SetPlaneConstraintEnabled(true);
	bUseControllerRotationYaw = false;

	static ConstructorHelpers::FObjectFinder<UTexture2D> DragonDayAsset(TEXT("/Game/Sprites/Dragon_Mascot_Day_Idle.Dragon_Mascot_Day_Idle"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> DragonNightAsset(TEXT("/Game/Sprites/Dragon_Mascot_Night_Idle.Dragon_Mascot_Night_Idle"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> RaccoonDayAsset(TEXT("/Game/Sprites/Racoon_Mascot_Day_Idle.Racoon_Mascot_Day_Idle"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> RaccoonNightAsset(TEXT("/Game/Sprites/Racoon_Mascot_Night_Idle.Racoon_Mascot_Night_Idle"));
	if (DragonDayAsset.Succeeded()) DragonDaySprite = DragonDayAsset.Object;
	if (DragonNightAsset.Succeeded()) DragonNightSprite = DragonNightAsset.Object;
	if (RaccoonDayAsset.Succeeded()) RaccoonDaySprite = RaccoonDayAsset.Object;
	if (RaccoonNightAsset.Succeeded()) RaccoonNightSprite = RaccoonNightAsset.Object;

	CharacterSprite = CreateDefaultSubobject<UBillboardComponent>(TEXT("CharacterSprite"));
	CharacterSprite->SetupAttachment(GetRootComponent());
	CharacterSprite->SetRelativeScale3D(FVector(0.7f));
	CharacterSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CharacterSprite->SetHiddenInGame(false);
	CharacterSprite->SetVisibility(true, true);
	UpdateSprite();
}

void ABoardwalkRoamingNPC::BeginPlay()
{
	Super::BeginPlay();
	PatrolCenter = GetActorLocation();
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	UpdateSprite();
}

void ABoardwalkRoamingNPC::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const float DistanceFromCenter = GetActorLocation().X - PatrolCenter.X;
	if (DistanceFromCenter >= PatrolRadius)
	{
		PatrolDirection = -1.0f;
		UpdateSprite();
	}
	else if (DistanceFromCenter <= -PatrolRadius)
	{
		PatrolDirection = 1.0f;
		UpdateSprite();
	}

	AddMovementInput(FVector::ForwardVector, PatrolDirection);
}

void ABoardwalkRoamingNPC::SetDirectorCutActive_Implementation(bool bNewIsDirectorCut)
{
	bIsDirectorCut = bNewIsDirectorCut;
	UpdateSprite();
}

void ABoardwalkRoamingNPC::ConfigurePatrol(float NewPatrolRadius, bool bStartMovingRight, int32 NewAppearanceIndex, bool bNewIsDirectorCut)
{
	PatrolRadius = FMath::Max(0.0f, NewPatrolRadius);
	PatrolDirection = bStartMovingRight ? 1.0f : -1.0f;
	AppearanceIndex = NewAppearanceIndex % 2;
	bIsDirectorCut = bNewIsDirectorCut;
	UpdateSprite();
}

void ABoardwalkRoamingNPC::UpdateSprite()
{
	if (!CharacterSprite)
	{
		return;
	}

	CharacterSprite->SetHiddenInGame(false);
	CharacterSprite->SetVisibility(true, true);
	UTexture2D* SpriteToUse = nullptr;
	if (AppearanceIndex == 0)
	{
		SpriteToUse = bIsDirectorCut ? DragonNightSprite : DragonDaySprite;
	}
	else
	{
		SpriteToUse = bIsDirectorCut ? RaccoonNightSprite : RaccoonDaySprite;
	}
	CharacterSprite->SetSprite(SpriteToUse);
	CharacterSprite->SetRelativeScale3D(FVector(PatrolDirection < 0.0f ? -0.7f : 0.7f, 0.7f, 0.7f));
}
