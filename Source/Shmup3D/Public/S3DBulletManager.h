// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "S3DBulletManager.generated.h"

UCLASS(Blueprintable)
class US3DBulletInstance : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	float CollisionRadius;

	UPROPERTY()
	uint32 bIsInWorldSpace : 1;
};

UCLASS(Blueprintable)
class US3DBulletTemplate : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere)
	UParticleSystem* DeathParticleSystem;

	UPROPERTY(EditAnywhere)
	uint32 bIsInWorldSpace : 1;

	UPROPERTY()
	TArray<US3DBulletInstance*> Instances;

	UPROPERTY()
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;
};

UCLASS(Blueprintable, BlueprintType)
class SHMUP3D_API AS3DBulletManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AS3DBulletManager(const FObjectInitializer& ObjectInitializer);

	// AActor overrides
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginDestroy() override;

	UFUNCTION()
	US3DBulletTemplate* SetupTemplate(TSubclassOf<US3DBulletTemplate> TemplateClass);

	UFUNCTION(BlueprintCallable, Category = "Bullets")
	void SpawnBullet(TSubclassOf<US3DBulletTemplate> TemplateClass, const FTransform& Transform, const FVector& Velocity, const float Size, const float CollisionRadius, const bool bIsInWorldSpace);

	UFUNCTION()
	virtual void MoveBullet(US3DBulletInstance* Instance);
	virtual bool CheckBulletCollision(FHitResult& HitResult, US3DBulletInstance* Instance, const FVector& PreviousLocation);

protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* RootSceneComponent;

	UPROPERTY()
	TMap<TSubclassOf<US3DBulletTemplate>, US3DBulletTemplate*> BulletTemplates;

	UPROPERTY()
	TArray<US3DBulletInstance*> InstancePool;
};
