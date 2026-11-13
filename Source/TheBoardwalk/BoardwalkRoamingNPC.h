// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BoardwalkWorldVariant.h"
#include "BoardwalkRoamingNPC.generated.h"

class UBillboardComponent;
class UTexture2D;

UCLASS()
class THEBOARDWALK_API ABoardwalkRoamingNPC : public ACharacter, public IBoardwalkWorldVariant
{
	GENERATED_BODY()

public:
	ABoardwalkRoamingNPC();

	virtual void Tick(float DeltaSeconds) override;
	virtual void SetDirectorCutActive_Implementation(bool bIsDirectorCut) override;

	void ConfigurePatrol(float PatrolRadius, bool bStartMovingRight, int32 AppearanceIndex, bool bIsDirectorCut);

protected:
	virtual void BeginPlay() override;

private:
	void UpdateSprite();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBillboardComponent> CharacterSprite;

	UPROPERTY(Transient)
	TObjectPtr<UTexture2D> DragonDaySprite;

	UPROPERTY(Transient)
	TObjectPtr<UTexture2D> DragonNightSprite;

	UPROPERTY(Transient)
	TObjectPtr<UTexture2D> RaccoonDaySprite;

	UPROPERTY(Transient)
	TObjectPtr<UTexture2D> RaccoonNightSprite;

	FVector PatrolCenter = FVector::ZeroVector;
	float PatrolRadius = 300.0f;
	float PatrolSpeed = 150.0f;
	float PatrolDirection = 1.0f;
	int32 AppearanceIndex = 0;
	bool bIsDirectorCut = false;
};
