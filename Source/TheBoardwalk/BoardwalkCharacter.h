// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BoardwalkCharacter.generated.h"

class UCameraComponent;
class UBillboardComponent;
class USpringArmComponent;
class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBoardwalkWorldChanged, bool, bIsDirectorCut);

UCLASS()
class THEBOARDWALK_API ABoardwalkCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABoardwalkCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category = "Boardwalk|World")
	bool IsDirectorCutActive() const { return bIsDirectorCut; }

	/** Level Blueprints can listen for presentation changes when the player switches cuts. */
	UPROPERTY(BlueprintAssignable, Category = "Boardwalk|World")
	FBoardwalkWorldChanged OnWorldChanged;

	/** Override in a Blueprint child to play a kick animation or sound. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Boardwalk|Combat")
	void OnKickStarted();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> SideCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBillboardComponent> CharacterSprite;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites")
	TObjectPtr<UTexture2D> DaySpriteTexture;

	UPROPERTY(Transient)
	TObjectPtr<UTexture2D> NativeDaySpriteTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites")
	TObjectPtr<UTexture2D> NightSpriteTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites")
	TObjectPtr<UTexture2D> DayJumpSpriteTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites")
	TObjectPtr<UTexture2D> NightJumpSpriteTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites")
	TObjectPtr<UTexture2D> DayJumpSpriteTextureLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites")
	TObjectPtr<UTexture2D> NightJumpSpriteTextureLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites")
	TObjectPtr<UTexture2D> DayRunSpriteTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites")
	TObjectPtr<UTexture2D> NightRunSpriteTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites")
	TObjectPtr<UTexture2D> DayRunSpriteTextureLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites")
	TObjectPtr<UTexture2D> NightRunSpriteTextureLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites|Movement", meta = (ClampMin = "1", UIMin = "1"))
	int32 RunAnimationColumns = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites|Movement", meta = (ClampMin = "1", UIMin = "1"))
	int32 RunAnimationRows = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites|Movement", meta = (ClampMin = "1.0", UIMin = "1.0"))
	float RunAnimationFramesPerSecond = 12.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites|Movement", meta = (ClampMin = "1", UIMin = "1"))
	int32 JumpAnimationColumns = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites|Movement", meta = (ClampMin = "1", UIMin = "1"))
	int32 JumpAnimationRows = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites|Movement", meta = (ClampMin = "1.0", UIMin = "1.0"))
	float JumpAnimationFramesPerSecond = 12.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites|Movement", meta = (DisplayName = "Jump Up First Frame", ClampMin = "0", UIMin = "0", UIMax = "15"))
	int32 JumpAnimationStartFrame = 6;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites|Movement", meta = (DisplayName = "Jump Up Last Frame", ClampMin = "0", UIMin = "0", UIMax = "15"))
	int32 JumpAnimationEndFrame = 8;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites|Movement", meta = (DisplayName = "Landing First Frame", ClampMin = "0", UIMin = "0", UIMax = "15"))
	int32 JumpLandingStartFrame = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites|Movement", meta = (DisplayName = "Landing Last Frame", ClampMin = "0", UIMin = "0", UIMax = "15"))
	int32 JumpLandingEndFrame = 12;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites|Movement", meta = (DisplayName = "Movement Sprite Scale Multiplier", ClampMin = "0.1", UIMin = "0.1", UIMax = "8.0"))
	float MovementSpriteScaleMultiplier = 4.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites|Movement", meta = (DisplayName = "Movement Sprite Offset", ToolTip = "Relative offset applied only while the movement animation is playing."))
	FVector MovementSpriteOffset = FVector(0.0f, 0.0f, 65.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites", meta = (DisplayName = "Sprite Scale", ClampMin = "0.01", UIMin = "0.1", UIMax = "2.0"))
	float CharacterSpriteScale = 0.7f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites")
	TObjectPtr<UTexture2D> DaySpriteTextureLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites")
	TObjectPtr<UTexture2D> NightSpriteTextureLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float WalkSpeed = 450.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction", meta = (ClampMin = "0.0"))
	float InteractionRadius = 220.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (ClampMin = "0.0"))
	float KickRadius = 150.0f;

private:
	void SetMoveLeft(bool bPressed);
	void SetMoveRight(bool bPressed);
	void MoveLeftPressed();
	void MoveLeftReleased();
	void MoveRightPressed();
	void MoveRightReleased();
	void InteractWithNearbyActor();
	void Kick();
	void SwitchWorld();
	void ApplyWorldVariant();
	void UpdateCharacterSprite();
	void FindAndCallNearbyInterface(float Radius, bool bForKick);

	bool bMoveLeftHeld = false;
	bool bMoveRightHeld = false;
	bool bIsDirectorCut = false;
	bool bIsMoving = false;
	bool bIsJumping = false;
	bool bIsJumpingUp = false;
	bool bIsLandingAnimation = false;
	float FacingDirection = 1.0f;
	float RunAnimationElapsed = 0.0f;
	int32 RunAnimationFrame = 0;
	float JumpAnimationElapsed = 0.0f;
	int32 JumpAnimationFrame = 0;
	FVector SpriteBaseRelativeLocation = FVector::ZeroVector;
};
