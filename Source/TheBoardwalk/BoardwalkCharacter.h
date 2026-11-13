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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Sprites")
	TObjectPtr<UTexture2D> NightSpriteTexture;

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
	void SpawnRoamingNPCs();
	void SpawnBoardwalkSetPieces();
	void UpdateCharacterSprite();
	void FindAndCallNearbyInterface(float Radius, bool bForKick);

	bool bMoveLeftHeld = false;
	bool bMoveRightHeld = false;
	bool bIsDirectorCut = false;
	float FacingDirection = 1.0f;
};
