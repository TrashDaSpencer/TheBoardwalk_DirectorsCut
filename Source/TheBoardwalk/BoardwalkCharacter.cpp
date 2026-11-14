// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoardwalkCharacter.h"

#include "BoardwalkInteractable.h"
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

	static ConstructorHelpers::FObjectFinder<UTexture2D> DayTextureAsset(TEXT("/Game/Player/Guy_Day_Idle.Guy_Day_Idle"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> NightTextureAsset(TEXT("/Game/Player/Guy_Night_Idle.Guy_Night_Idle"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> DayJumpTextureAsset(TEXT("/Game/Player/Guy_Day_Jump.Guy_Day_Jump"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> NightJumpTextureAsset(TEXT("/Game/Player/Guy_Night_Jump.Guy_Night_Jump"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> DayTextureLeftAsset(TEXT("/Game/Player/Guy_Day_Idle_Left.Guy_Day_Idle_Left"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> NightTextureLeftAsset(TEXT("/Game/Player/Guy_Night_Idle_Left.Guy_Night_Idle_Left"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> DayRunTextureAsset(TEXT("/Game/Player/Guy_Day_Run.Guy_Day_Run"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> NightRunTextureAsset(TEXT("/Game/Player/Guy_Night_Run.Guy_Night_Run"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> DayRunTextureLeftAsset(TEXT("/Game/Player/Guy_Day_Run_Left.Guy_Day_Run_Left"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> NightRunTextureLeftAsset(TEXT("/Game/Player/Guy_Night_Run_Left.Guy_Night_Run_Left"));
	if (DayTextureAsset.Succeeded())
	{
		DaySpriteTexture = DayTextureAsset.Object;
		NativeDaySpriteTexture = DayTextureAsset.Object;
	}
	if (NightTextureAsset.Succeeded())
	{
		NightSpriteTexture = NightTextureAsset.Object;
	}
	if (DayJumpTextureAsset.Succeeded())
	{
		DayJumpSpriteTexture = DayJumpTextureAsset.Object;
	}
	if (NightJumpTextureAsset.Succeeded())
	{
		NightJumpSpriteTexture = NightJumpTextureAsset.Object;
	}
	if (DayTextureLeftAsset.Succeeded())
	{
		DaySpriteTextureLeft = DayTextureLeftAsset.Object;
	}
	if (NightTextureLeftAsset.Succeeded())
	{
		NightSpriteTextureLeft = NightTextureLeftAsset.Object;
	}
	if (DayRunTextureAsset.Succeeded())
	{
		DayRunSpriteTexture = DayRunTextureAsset.Object;
	}
	if (NightRunTextureAsset.Succeeded())
	{
		NightRunSpriteTexture = NightRunTextureAsset.Object;
	}
	if (DayRunTextureLeftAsset.Succeeded())
	{
		DayRunSpriteTextureLeft = DayRunTextureLeftAsset.Object;
	}
	if (NightRunTextureLeftAsset.Succeeded())
	{
		NightRunSpriteTextureLeft = NightRunTextureLeftAsset.Object;
	}

	CharacterSprite = CreateDefaultSubobject<UBillboardComponent>(TEXT("CharacterSprite"));
	CharacterSprite->SetupAttachment(GetRootComponent());
	CharacterSprite->SetRelativeScale3D(FVector(CharacterSpriteScale));
	CharacterSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CharacterSprite->SetHiddenInGame(false);
	CharacterSprite->SetVisibility(true, true);
	UpdateCharacterSprite();
}

void ABoardwalkCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	if (CharacterSprite)
	{
		SpriteBaseRelativeLocation = CharacterSprite->GetRelativeLocation();
	}
	if (CharacterSprite && CharacterSprite->Sprite && CharacterSprite->Sprite != NativeDaySpriteTexture && DaySpriteTexture == NativeDaySpriteTexture)
	{
		DaySpriteTexture = CharacterSprite->Sprite;
	}
	ApplyWorldVariant();
}

void ABoardwalkCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const float Direction = (bMoveRightHeld ? 1.0f : 0.0f) - (bMoveLeftHeld ? 1.0f : 0.0f);
	const bool bShouldMove = !FMath::IsNearlyZero(Direction);
	if (bShouldMove)
	{
		if (FacingDirection != Direction)
		{
			FacingDirection = Direction;
			UpdateCharacterSprite();
		}
		SetActorRotation(FRotator(0.0f, FacingDirection > 0.0f ? 0.0f : 180.0f, 0.0f));
		AddMovementInput(FVector::ForwardVector, Direction);
	}

	if (bShouldMove != bIsMoving)
	{
		bIsMoving = bShouldMove;
		RunAnimationElapsed = 0.0f;
		RunAnimationFrame = 0;
		UpdateCharacterSprite();
	}

	const bool bShouldJump = GetCharacterMovement()->IsFalling();
	if (bShouldJump != bIsJumping)
	{
		const bool bJustLanded = bIsJumping && !bShouldJump;
		bIsJumping = bShouldJump;
		bIsJumpingUp = bIsJumping;
		bIsLandingAnimation = bJustLanded;
		JumpAnimationElapsed = 0.0f;
		const int32 FrameCount = FMath::Max(JumpAnimationColumns * JumpAnimationRows, 1);
		const int32 StartFrame = FMath::Clamp(bJustLanded ? JumpLandingStartFrame : JumpAnimationStartFrame, 0, FrameCount - 1);
		JumpAnimationFrame = bIsJumping ? StartFrame : 0;
		if (bJustLanded)
		{
			JumpAnimationFrame = StartFrame;
		}
		UpdateCharacterSprite();
	}

	if (bIsMoving && !bIsJumping)
	{
		const float FrameDuration = 1.0f / FMath::Max(RunAnimationFramesPerSecond, 1.0f);
		RunAnimationElapsed += DeltaSeconds;
		if (RunAnimationElapsed >= FrameDuration)
		{
			RunAnimationElapsed = FMath::Fmod(RunAnimationElapsed, FrameDuration);
			RunAnimationFrame = (RunAnimationFrame + 1) % FMath::Max(RunAnimationColumns * RunAnimationRows, 1);
			UpdateCharacterSprite();
		}
	}

	if (bIsJumping)
	{
		if (bIsJumpingUp && GetVelocity().Z <= 0.0f)
		{
			bIsJumpingUp = false;
			JumpAnimationElapsed = 0.0f;
		}

		if (bIsJumpingUp)
		{
			const float FrameDuration = 1.0f / FMath::Max(JumpAnimationFramesPerSecond, 1.0f);
			JumpAnimationElapsed += DeltaSeconds;
			if (JumpAnimationElapsed >= FrameDuration)
			{
				JumpAnimationElapsed = FMath::Fmod(JumpAnimationElapsed, FrameDuration);
				const int32 FrameCount = FMath::Max(JumpAnimationColumns * JumpAnimationRows, 1);
				const int32 StartFrame = FMath::Clamp(JumpAnimationStartFrame, 0, FrameCount - 1);
				const int32 EndFrame = FMath::Clamp(JumpAnimationEndFrame, StartFrame, FrameCount - 1);
				const int32 PreviousFrame = JumpAnimationFrame;
				JumpAnimationFrame = FMath::Min(JumpAnimationFrame + 1, EndFrame);
				if (JumpAnimationFrame != PreviousFrame)
				{
					UpdateCharacterSprite();
				}
			}
		}
	}

	if (bIsLandingAnimation)
	{
		const float FrameDuration = 1.0f / FMath::Max(JumpAnimationFramesPerSecond, 1.0f);
		JumpAnimationElapsed += DeltaSeconds;
		if (JumpAnimationElapsed >= FrameDuration)
		{
			JumpAnimationElapsed = FMath::Fmod(JumpAnimationElapsed, FrameDuration);
			const int32 FrameCount = FMath::Max(JumpAnimationColumns * JumpAnimationRows, 1);
			const int32 StartFrame = FMath::Clamp(JumpLandingStartFrame, 0, FrameCount - 1);
			const int32 EndFrame = FMath::Clamp(JumpLandingEndFrame, StartFrame, FrameCount - 1);
			if (JumpAnimationFrame < EndFrame)
			{
				++JumpAnimationFrame;
				UpdateCharacterSprite();
			}
			else
			{
				bIsLandingAnimation = false;
				UpdateCharacterSprite();
			}
		}
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

void ABoardwalkCharacter::UpdateCharacterSprite()
{
	if (CharacterSprite)
	{
		CharacterSprite->SetHiddenInGame(false);
		CharacterSprite->SetVisibility(true, true);

		const bool bFacingLeft = FacingDirection < 0.0f;
		UTexture2D* SpriteToUse = bIsDirectorCut
			? (bFacingLeft ? NightSpriteTextureLeft : NightSpriteTexture)
			: (bFacingLeft ? DaySpriteTextureLeft : DaySpriteTexture);
		if (bIsJumping || bIsLandingAnimation)
		{
			UTexture2D* JumpSpriteToUse = bIsDirectorCut
				? (bFacingLeft && NightJumpSpriteTextureLeft ? NightJumpSpriteTextureLeft : NightJumpSpriteTexture)
				: (bFacingLeft && DayJumpSpriteTextureLeft ? DayJumpSpriteTextureLeft : DayJumpSpriteTexture);
			if (JumpSpriteToUse && JumpAnimationColumns > 0 && JumpAnimationRows > 0)
			{
				const int32 FrameWidth = JumpSpriteToUse->GetSizeX() / JumpAnimationColumns;
				const int32 FrameHeight = JumpSpriteToUse->GetSizeY() / JumpAnimationRows;
				if (FrameWidth > 0 && FrameHeight > 0)
				{
					const int32 Column = JumpAnimationFrame % JumpAnimationColumns;
					const int32 Row = JumpAnimationFrame / JumpAnimationColumns;
					CharacterSprite->SetSpriteAndUV(JumpSpriteToUse, Column * FrameWidth, FrameWidth, Row * FrameHeight, FrameHeight);
				}
				else
				{
					CharacterSprite->SetSpriteAndUV(SpriteToUse, 0, 0, 0, 0);
				}
			}
			else
			{
				CharacterSprite->SetSpriteAndUV(SpriteToUse, 0, 0, 0, 0);
			}
		}
		else if (bIsMoving)
		{
			UTexture2D* RunSpriteToUse = bIsDirectorCut
				? (bFacingLeft ? NightRunSpriteTextureLeft : NightRunSpriteTexture)
				: (bFacingLeft ? DayRunSpriteTextureLeft : DayRunSpriteTexture);
			if (RunSpriteToUse && RunAnimationColumns > 0 && RunAnimationRows > 0)
			{
				const int32 FrameWidth = RunSpriteToUse->GetSizeX() / RunAnimationColumns;
				const int32 FrameHeight = RunSpriteToUse->GetSizeY() / RunAnimationRows;
				if (FrameWidth > 0 && FrameHeight > 0)
				{
					const int32 Column = RunAnimationFrame % RunAnimationColumns;
					const int32 Row = RunAnimationFrame / RunAnimationColumns;
					CharacterSprite->SetSpriteAndUV(RunSpriteToUse, Column * FrameWidth, FrameWidth, Row * FrameHeight, FrameHeight);
				}
				else
				{
					CharacterSprite->SetSpriteAndUV(SpriteToUse, 0, 0, 0, 0);
				}
			}
			else
			{
				CharacterSprite->SetSpriteAndUV(SpriteToUse, 0, 0, 0, 0);
			}
		}
		else
		{
			CharacterSprite->SetSpriteAndUV(SpriteToUse, 0, 0, 0, 0);
		}
		const float ActiveSpriteScale = CharacterSpriteScale * ((bIsMoving || bIsJumping || bIsLandingAnimation) ? MovementSpriteScaleMultiplier : 1.0f);
		CharacterSprite->SetRelativeScale3D(FVector(ActiveSpriteScale));
		CharacterSprite->SetRelativeLocation(SpriteBaseRelativeLocation + (bIsMoving && !bIsJumping && !bIsLandingAnimation ? MovementSpriteOffset : FVector::ZeroVector));
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
