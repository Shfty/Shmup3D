// Fill out your copyright notice in the Description page of Project Settings.

#include "Shmup3D.h"

#include "S3DEnemy.h"
#include "S3DGameState.h"

#include "S3DEnemyMovementComponent.h"

US3DEnemyMovementComponent::US3DEnemyMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	MaxSpeed(1200.0f),
	Acceleration(4000.0f),
	Deceleration(8000.0f),
	bRotationFollowsVelocity(true),
	bPositionCorrected(false),
	CachedOrientation(FRotator::ZeroRotator)
{
	bWantsBeginPlay = true;
	bAutoActivate = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void US3DEnemyMovementComponent::BeginPlay()
{
	CachedOrientation = GetOwner()->GetActorRotation();

	Super::BeginPlay();
}

void US3DEnemyMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if(ShouldSkipUpdate(DeltaTime) || !UpdatedComponent)
	{
		return;
	}

	// Apply input velocity
	ApplyControlInputToVelocity(DeltaTime);

	// Limit maximum speed
	if(IsExceedingMaxSpeed(MaxSpeed) == true)
	{
		Velocity = Velocity.GetUnsafeNormal() * MaxSpeed;
	}

	bPositionCorrected = false;

	// Move actor
	FVector Delta = Velocity * DeltaTime;

	if(!Delta.IsNearlyZero(1e-6f))
	{
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();
		const FQuat Rotation = (bRotationFollowsVelocity && !Velocity.IsNearlyZero(0.01f)) ? Velocity.ToOrientationQuat() : UpdatedComponent->GetComponentQuat();

		FHitResult Hit(1.f);
		SafeMoveUpdatedComponent(Delta, Rotation, true, Hit);

		if(Hit.IsValidBlockingHit())
		{
			HandleImpact(Hit, DeltaTime, Delta);
			// Try to slide the remaining distance along the surface.
			SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
		}

		// Update velocity
		// We don't want position changes to vastly reverse our direction (which can happen due to penetration fixups etc)
		if(!bPositionCorrected)
		{
			const FVector NewLocation = UpdatedComponent->GetComponentLocation();
			Velocity = ((NewLocation - OldLocation) / DeltaTime);
		}

		// Finalize
		UpdateComponentVelocity();
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void US3DEnemyMovementComponent::Activate(bool bReset /*= false*/)
{
	SetComponentTickEnabled(true);

	ActivationTimestamp = GetWorld()->GetTimeSeconds();

	if(bMoveRelativeToPlayVolume)
	{
		AS3DGameState* GameState = StaticCast<AS3DGameState*>(GetWorld()->GetGameState());
		if(GameState != nullptr)
		{
			if(GameState->PlayVolume != nullptr)
			{
				GetOwner()->AttachToActor(GameState->PlayVolume, FAttachmentTransformRules::KeepWorldTransform);
			}
		}
	}

	Super::Activate(bReset);
}

void US3DEnemyMovementComponent::Deactivate()
{
	SetComponentTickEnabled(false);

	if(bMoveRelativeToPlayVolume)
	{
		GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	Super::Deactivate();
}

FVector US3DEnemyMovementComponent::CalculateControlInput_Implementation(float DeltaTime)
{
	return FVector::ZeroVector;
}

void US3DEnemyMovementComponent::ApplyControlInputToVelocity(float DeltaTime)
{
	const FVector ControlAcceleration = CalculateControlInput(DeltaTime);

	const float AnalogInputModifier = (ControlAcceleration.SizeSquared() > 0.f ? ControlAcceleration.Size() : 0.f);
	const float MaxPawnSpeed = GetMaxSpeed() * AnalogInputModifier;
	const bool bExceedingMaxSpeed = IsExceedingMaxSpeed(MaxPawnSpeed);

	if(AnalogInputModifier > 0.f && !bExceedingMaxSpeed)
	{
		// Apply change in velocity direction
		if(Velocity.SizeSquared() > 0.f)
		{
			// Change direction faster than only using acceleration, but never increase velocity magnitude.
			Velocity = Velocity + (ControlAcceleration * Velocity.Size() - Velocity) * DeltaTime;
		}
	}
	else
	{
		// Dampen velocity magnitude based on deceleration.
		if(Velocity.SizeSquared() > 0.f)
		{
			const FVector OldVelocity = Velocity;
			const float VelSize = FMath::Max(Velocity.Size() - FMath::Abs(Deceleration) * DeltaTime, 0.f);
			Velocity = Velocity.GetSafeNormal() * VelSize;

			// Don't allow braking to lower us below max speed if we started above it.
			if(bExceedingMaxSpeed && Velocity.SizeSquared() < FMath::Square(MaxPawnSpeed))
			{
				Velocity = OldVelocity.GetSafeNormal() * MaxPawnSpeed;
			}
		}
	}

	// Apply acceleration and clamp velocity magnitude.
	const float NewMaxSpeed = (IsExceedingMaxSpeed(MaxPawnSpeed)) ? Velocity.Size() : MaxPawnSpeed;
	Velocity += ControlAcceleration * FMath::Abs(Acceleration) * DeltaTime;
	Velocity = Velocity.GetClampedToMaxSize(NewMaxSpeed);
}

bool US3DEnemyMovementComponent::ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotationQuat)
{
	bPositionCorrected |= Super::ResolvePenetrationImpl(Adjustment, Hit, NewRotationQuat);
	return bPositionCorrected;
}

float US3DEnemyMovementComponent::GetSecondsSinceActivation()
{
	return GetWorld()->GetTimeSeconds() - ActivationTimestamp;
}
