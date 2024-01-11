// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DamageType.h"
#include "S3DDamageType.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SHMUP3D_API US3DDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	US3DDamageType();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	uint32 bRefillItanoAmmoOnKill : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	int32 ItanoRefillOnHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float ShockCannonChargeOnBulletHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	int32 Hitstop;
};
