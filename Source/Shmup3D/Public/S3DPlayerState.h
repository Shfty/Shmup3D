// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "S3DPlayerState.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SHMUP3D_API AS3DPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AS3DPlayerState();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Player State")
	void RechargeShockCannons(float Amount);

public:
	float ShockCannonRechargeRate;
};
