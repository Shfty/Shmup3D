// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/MovementComponent.h"
#include "S3DEnemyMovementComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class SHMUP3D_API US3DEnemyMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
	
public:
	US3DEnemyMovementComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void Activate(bool bReset = false) override;
	virtual void Deactivate() override;

	virtual float GetMaxSpeed() const override { return MaxSpeed; }
	virtual bool ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotationQuat) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Enemy Movement")
	FVector CalculateControlInput(float DeltaTime);

	void ApplyControlInputToVelocity(float DeltaTime);

	UFUNCTION(BlueprintPure, Category = "Enemy Movement")
	float GetSecondsSinceActivation();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Movement")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Movement")
	float Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Movement")
	float Deceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Movement")
	uint32 bRotationFollowsVelocity : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Movement")
	uint32 bMoveRelativeToPlayVolume : 1;

protected:
	UPROPERTY(BlueprintReadOnly)
	FRotator CachedOrientation;

	UPROPERTY(BlueprintReadOnly)
	float ActivationTimestamp;

private:
	UPROPERTY()
	uint32 bPositionCorrected : 1;
};
