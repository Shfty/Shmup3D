// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AS3DEnemy;

#include "GameFramework/GameState.h"
#include "S3DGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBulletParticleComponentAddedDelegate, US3DParticleSystemComponent*, ParticleSystemComponent);

UCLASS()
class SHMUP3D_API AS3DGameState : public AGameState
{
	GENERATED_BODY()

public:
	AS3DGameState(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Bullet Particles")
	class US3DParticleSystemComponent* GetBulletParticleComponent(UParticleSystem* Template,
																  UMaterialInterface* Material,
																  bool bUseLocalSpace,
																  float BulletLifetime,
																  float BulletSize,
																  float BulletCollisionSize);
	
public:
	UPROPERTY(BlueprintReadWrite, Transient)
	AActor* PlayVolume;

	UPROPERTY(BlueprintReadWrite, Transient)
	TArray<AS3DEnemy*> EnemiesInPlayVolume;

	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<US3DParticleSystemComponent*> BulletParticleComponents;

	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<FVector> BulletLocations;

	UPROPERTY(BlueprintAssignable)
	FBulletParticleComponentAddedDelegate OnBulletParticleComponentAdded;
};