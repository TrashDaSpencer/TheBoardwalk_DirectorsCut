// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoardwalkCharacter.h"

#include "BoardwalkBuilding.h"
#include "BoardwalkInteractable.h"
#include "BoardwalkPedestrianLine.h"
#include "BoardwalkRoamingNPC.h"
#include "BoardwalkWorldVariant.h"
#include "Components/BillboardComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/OverlapResult.h"
#include "Engine/Texture2D.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputCoreTypes.h"
#include "UObject/ConstructorHelpers.h"

ABoardwalkCharacter::ABoardwalkCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(34.0f, 92.0f);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->JumpZVelocity = 620.0f;
	GetCharacterMovement()->AirControl = 0.65f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 1.0f, 0.0f));
	GetCharacterMovement()->SetPlaneConstraintOrigin(FVector::ZeroVector);
	GetCharacterMovement()->SetPlaneConstraintEnabled(true);

	bUseControllerRotationYaw = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	CameraBoom->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	CameraBoom->TargetArmLength = 1400.0f;
	CameraBoom->bUsePawnControlRotation = false;

	SideCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SideCamera"));
	SideCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideCamera->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideCamera->OrthoWidth = 1800.0f;

	static ConstructorHelpers::FObjectFinder<UTexture2D> DayTextureAsset(TEXT("/Game/Sprites/Guy_Day_Idle.Guy_Day_Idle"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> NightTextureAsset(TEXT("/Game/Sprites/Guy_Night_Idle.Guy_Night_Idle"));
	if (DayTextureAsset.Succeeded())
	{
		DaySpriteTexture = DayTextureAsset.Object;
	}
	if (NightTextureAsset.Succeeded())
	{
		NightSpriteTexture = NightTextureAsset.Object;
	}

	CharacterSprite = CreateDefaultSubobject<UBillboardComponent>(TEXT("CharacterSprite"));
	CharacterSprite->SetupAttachment(GetRootComponent());
	CharacterSprite->SetRelativeScale3D(FVector(0.7f));
	CharacterSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CharacterSprite->SetHiddenInGame(false);
	CharacterSprite->SetVisibility(true, true);
	UpdateCharacterSprite();
}

void ABoardwalkCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	ApplyWorldVariant();
	SpawnRoamingNPCs();
	SpawnBoardwalkSetPieces();
}

void ABoardwalkCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const float Direction = (bMoveRightHeld ? 1.0f : 0.0f) - (bMoveLeftHeld ? 1.0f : 0.0f);
	if (!FMath::IsNearlyZero(Direction))
	{
		FacingDirection = Direction;
		SetActorRotation(FRotator(0.0f, FacingDirection > 0.0f ? 0.0f : 180.0f, 0.0f));
		UpdateCharacterSprite();
		AddMovementInput(FVector::ForwardVector, Direction);
	}
}

void ABoardwalkCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindKey(EKeys::A, IE_Pressed, this, &ABoardwalkCharacter::MoveLeftPressed);
	PlayerInputComponent->BindKey(EKeys::A, IE_Released, this, &ABoardwalkCharacter::MoveLeftReleased);
	PlayerInputComponent->BindKey(EKeys::D, IE_Pressed, this, &ABoardwalkCharacter::MoveRightPressed);
	PlayerInputComponent->BindKey(EKeys::D, IE_Released, this, &ABoardwalkCharacter::MoveRightReleased);
	PlayerInputComponent->BindKey(EKeys::SpaceBar, IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindKey(EKeys::SpaceBar, IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindKey(EKeys::LeftControl, IE_Pressed, this, &ABoardwalkCharacter::Kick);
	PlayerInputComponent->BindKey(EKeys::E, IE_Pressed, this, &ABoardwalkCharacter::InteractWithNearbyActor);
	PlayerInputComponent->BindKey(EKeys::Tab, IE_Pressed, this, &ABoardwalkCharacter::SwitchWorld);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->bShowMouseCursor = false;
	}
}

void ABoardwalkCharacter::SetMoveLeft(bool bPressed)
{
	bMoveLeftHeld = bPressed;
}

void ABoardwalkCharacter::SetMoveRight(bool bPressed)
{
	bMoveRightHeld = bPressed;
}

void ABoardwalkCharacter::MoveLeftPressed()
{
	SetMoveLeft(true);
}

void ABoardwalkCharacter::MoveLeftReleased()
{
	SetMoveLeft(false);
}

void ABoardwalkCharacter::MoveRightPressed()
{
	SetMoveRight(true);
}

void ABoardwalkCharacter::MoveRightReleased()
{
	SetMoveRight(false);
}

void ABoardwalkCharacter::InteractWithNearbyActor()
{
	FindAndCallNearbyInterface(InteractionRadius, false);
}

void ABoardwalkCharacter::Kick()
{
	OnKickStarted();
	FindAndCallNearbyInterface(KickRadius, true);
}

void ABoardwalkCharacter::SwitchWorld()
{
	bIsDirectorCut = !bIsDirectorCut;
	ApplyWorldVariant();
	OnWorldChanged.Broadcast(bIsDirectorCut);
}

void ABoardwalkCharacter::ApplyWorldVariant()
{
	UpdateCharacterSprite();
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor != this && Actor->GetClass()->ImplementsInterface(UBoardwalkWorldVariant::StaticClass()))
		{
			IBoardwalkWorldVariant::Execute_SetDirectorCutActive(Actor, bIsDirectorCut);
		}
	}
}

void ABoardwalkCharacter::SpawnRoamingNPCs()
{
	if (!GetWorld())
	{
		return;
	}

	const float SpawnOffsets[] = { -500.0f, 500.0f };
	for (int32 Index = 0; Index < UE_ARRAY_COUNT(SpawnOffsets); ++Index)
	{
		const FVector SpawnLocation = GetActorLocation() + FVector(SpawnOffsets[Index], 0.0f, 0.0f);
		const FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		if (ABoardwalkRoamingNPC* NPC = GetWorld()->SpawnActor<ABoardwalkRoamingNPC>(ABoardwalkRoamingNPC::StaticClass(), SpawnTransform, SpawnParameters))
		{
			NPC->ConfigurePatrol(250.0f, Index == 0, Index, bIsDirectorCut);
		}
	}
}

void ABoardwalkCharacter::SpawnBoardwalkSetPieces()
{
	if (!GetWorld())
	{
		return;
	}

	const float BuildingOffsets[] = { -700.0f, 650.0f };
	for (int32 Index = 0; Index < UE_ARRAY_COUNT(BuildingOffsets); ++Index)
	{
		const FVector BuildingLocation = GetActorLocation() + FVector(BuildingOffsets[Index], -450.0f, 0.0f);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (ABoardwalkBuilding* Building = GetWorld()->SpawnActor<ABoardwalkBuilding>(ABoardwalkBuilding::StaticClass(), FTransform(FRotator(0.0f, 180.0f, 0.0f), BuildingLocation), SpawnParameters))
		{
			Building->SetBuildingStyle(Index, bIsDirectorCut);
		}
	}

	const FVector LineLocation = GetActorLocation() + FVector(1050.0f, 0.0f, 0.0f);
	FActorSpawnParameters LineSpawnParameters;
	LineSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ABoardwalkPedestrianLine* PedestrianLine = GetWorld()->SpawnActor<ABoardwalkPedestrianLine>(ABoardwalkPedestrianLine::StaticClass(), FTransform(FRotator::ZeroRotator, LineLocation), LineSpawnParameters))
	{
		PedestrianLine->SetDirectorCutActive_Implementation(bIsDirectorCut);
	}
}

void ABoardwalkCharacter::UpdateCharacterSprite()
{
	if (CharacterSprite)
	{
		CharacterSprite->SetHiddenInGame(false);
		CharacterSprite->SetVisibility(true, true);

		const bool bFacingLeft = FacingDirection < 0.0f;
		UTexture2D* SpriteToUse = bIsDirectorCut ? NightSpriteTexture : DaySpriteTexture;
		CharacterSprite->SetSprite(SpriteToUse);
		CharacterSprite->SetRelativeScale3D(FVector(bFacingLeft ? -0.7f : 0.7f, 0.7f, 0.7f));
	}
}

void ABoardwalkCharacter::FindAndCallNearbyInterface(float Radius, bool bForKick)
{
	if (!GetWorld())
	{
		return;
	}

	const FVector Origin = GetActorLocation() + FVector(FacingDirection * Radius * 0.45f, 0.0f, 0.0f);
	FCollisionObjectQueryParams ObjectTypes;
	ObjectTypes.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectTypes.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectTypes.AddObjectTypesToQuery(ECC_Pawn);
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(BoardwalkPlayerAction), false, this);

	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, ObjectTypes, FCollisionShape::MakeSphere(Radius), QueryParams);

	AActor* ClosestActor = nullptr;
	float ClosestDistanceSquared = TNumericLimits<float>::Max();
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* Candidate = Overlap.GetActor();
		if (!Candidate || !Candidate->GetClass()->ImplementsInterface(UBoardwalkInteractable::StaticClass()))
		{
			continue;
		}

		const float DistanceSquared = FVector::DistSquared(GetActorLocation(), Candidate->GetActorLocation());
		if (DistanceSquared < ClosestDistanceSquared)
		{
			ClosestDistanceSquared = DistanceSquared;
			ClosestActor = Candidate;
		}
	}

	if (ClosestActor)
	{
		if (bForKick)
		{
			IBoardwalkInteractable::Execute_ReceivePlayerKick(ClosestActor, this);
		}
		else
		{
			IBoardwalkInteractable::Execute_Interact(ClosestActor, this);
		}
	}
}
