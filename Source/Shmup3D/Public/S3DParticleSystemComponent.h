// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class US3DDamageType;

#include "S3DParticleSystemComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Rendering, Common), hidecategories = Object, hidecategories = Physics, hidecategories = Collision, showcategories = Trigger, editinlinenew, meta = (BlueprintSpawnableComponent, DisplayName = "Bullet Particle Component"))
class SHMUP3D_API US3DParticleSystemComponent : public UParticleSystemComponent
{
	GENERATED_BODY()

public:
	US3DParticleSystemComponent(const FObjectInitializer& ObjectInitializer);

	/* Begin UActorComponent Overrides */
	virtual void InitializeComponent() override;
	/* End UActorComponent Overrides*/

	/* Begin UParticleSystemComponent overrides */
	virtual bool ParticleLineCheck(FHitResult& Hit, AActor* SourceActor, const FVector& End, const FVector& Start, const FVector& HalfExtent, const FCollisionObjectQueryParams& ObjectParams) override;
	/* End UParticleSystemComponent overrides */

	UFUNCTION(BlueprintCallable, Category = "Bullet Particles")
	void SpawnBulletFromActor(AActor* SourceActor, float Velocity);

	UFUNCTION(BlueprintCallable, Category = "Bullet Particles")
	void SpawnBulletAtTransform(const FTransform& Transform, const FVector& Velocity);

	FParticleEmitterInstance* GetEmitterInstanceByName(const FName& Name);

protected:
	UFUNCTION()
	void HandleParticleCollision(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction, FVector Normal, FName BoneName, UPhysicalMaterial* PhysMat);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* BulletMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bUseLocalSpace : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletLifetime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletCollisionSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<US3DDamageType> BulletDamageType;

private:
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* DynamicMaterial;
};
