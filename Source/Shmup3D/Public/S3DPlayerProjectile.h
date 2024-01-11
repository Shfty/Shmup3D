// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class US3DDamageType;

#include "GameFramework/Actor.h"
#include "S3DPlayerProjectile.generated.h"

UCLASS(Blueprintable, BlueprintType)
class SHMUP3D_API AS3DPlayerProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AS3DPlayerProjectile();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<US3DDamageType> DamageType;
};
