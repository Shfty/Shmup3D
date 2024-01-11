// Fill out your copyright notice in the Description page of Project Settings.

#include "Shmup3D.h"

#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleLODLevel.h"
#include "Particles/ParticleModuleRequired.h"

#include "S3DPlayerProjectile.h"
#include "S3DPlayerState.h"
#include "S3DDamageType.h"

#include "S3DParticleSystemComponent.h"

US3DParticleSystemComponent::US3DParticleSystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	bUseLocalSpace(true),
	BulletLifetime(1000.0f),
	BulletSize(200.0f),
	BulletCollisionSize(200.0f),
	BulletDamageType(nullptr)
{
	static ConstructorHelpers::FClassFinder<US3DDamageType> DamageTypeClass(TEXT("/Game/Blueprints/DamageTypes/BP_EnemyBulletDamage"));
	if(DamageTypeClass.Succeeded())
	{
		BulletDamageType = DamageTypeClass.Class;
	}

	SetRenderCustomDepth(true);
	SetCustomDepthStencilValue(255);
	bWantsInitializeComponent = true;
}

void US3DParticleSystemComponent::InitializeComponent()
{
	// TODO: Replace this with procedural emitter generation
	{
		// Setup particle system parameters
		FParticleSysParam LifetimeParam;
		LifetimeParam.Name = TEXT("Lifetime");
		LifetimeParam.ParamType = EParticleSysParamType::PSPT_Scalar;
		LifetimeParam.Scalar = BulletLifetime;
		InstanceParameters.Add(LifetimeParam);

		FParticleSysParam SizeParam;
		SizeParam.Name = TEXT("Size");
		SizeParam.ParamType = EParticleSysParamType::PSPT_Scalar;
		SizeParam.Scalar = BulletSize;
		InstanceParameters.Add(SizeParam);

		// Setup particle system material
		if(BulletMaterial != nullptr)
		{
			DynamicMaterial = CreateNamedDynamicMaterialInstance(TEXT("BulletMaterial"), BulletMaterial);
		}
	}

	// Setup particle collision handler
	OnParticleCollide.AddDynamic(this, &US3DParticleSystemComponent::HandleParticleCollision);

	Super::InitializeComponent();
}

bool US3DParticleSystemComponent::ParticleLineCheck(FHitResult& Hit, AActor* SourceActor, const FVector& End, const FVector& Start, const FVector& HalfExtent, const FCollisionObjectQueryParams& ObjectParams)
{
	FCollisionQueryParams QueryParams;
	QueryParams.TraceTag = FName(TEXT("ParticleCollision"));
	QueryParams.AddIgnoredActor(SourceActor);

	FlushPersistentDebugLines(GetWorld());

	if(Start == End)
	{
		TArray<FOverlapResult> Overlaps;
		bool DidOverlap = GetWorld()->OverlapMultiByObjectType(Overlaps,
															   Start,
															   FQuat::Identity,
															   ObjectParams,
															   FCollisionShape::MakeSphere(BulletCollisionSize * 0.5f),
															   QueryParams);

		if(DidOverlap && Overlaps.Num() > 0)
		{
			FOverlapResult& Overlap = Overlaps[0];
			Hit.bBlockingHit = Overlap.bBlockingHit;
			Hit.Actor = Overlap.Actor;
			Hit.Component = Overlap.Component;
			Hit.Location = End;
		}

		return DidOverlap;
	}
	else
	{
		// Undo the endpoint calculation done in ParticleModules_Collision in order to get the particle's location
		// We don't need it, because we're doing swept sphere checks with a known collision radius
		FVector Direction = (End - Start);
		Direction.Normalize();
		FVector AdjustedEnd = End - Direction * BulletSize;

		return GetWorld()->SweepSingleByObjectType(Hit,
												   Start,
												   AdjustedEnd,
												   FQuat::Identity,
												   ObjectParams,
												   FCollisionShape::MakeSphere(BulletCollisionSize * 0.5f),
												   QueryParams);
	}
}

void US3DParticleSystemComponent::SpawnBulletFromActor(AActor* SourceActor, float Velocity)
{
	FTransform ParentTransform = SourceActor->GetTransform();
	SpawnBulletAtTransform(ParentTransform, FVector::ForwardVector * Velocity);
}

void US3DParticleSystemComponent::SpawnBulletAtTransform(const FTransform& Transform, const FVector& Velocity)
{
	if(bIsActive)
	{
		FTransform FinalTransform = FTransform::Identity;
		FVector FinalVelocity = FVector::ZeroVector;
		FName EventName = bUseLocalSpace ? TEXT("FireLocal") : TEXT("FireWorld");

		if(bUseLocalSpace)
		{
			FinalTransform = Transform.GetRelativeTransform(GetOwner()->GetTransform());
			FinalVelocity = FinalTransform.TransformVectorNoScale(Velocity);
		}
		else
		{
			FinalTransform = Transform;
			FinalVelocity = FinalTransform.TransformVectorNoScale(Velocity);
		}

		GenerateParticleEvent(EventName, 0.0f, FinalTransform.GetLocation(), FinalTransform.GetRotation().GetForwardVector(), FinalVelocity);
	}
}

FParticleEmitterInstance* US3DParticleSystemComponent::GetEmitterInstanceByName(const FName& Name)
{
	FParticleEmitterInstance** FoundInstance = EmitterInstances.FindByPredicate([&](FParticleEmitterInstance* EmitterInstance) {
		return EmitterInstance->SpriteTemplate->GetEmitterName() == Name;
	});

	if(FoundInstance != nullptr)
	{
		return *FoundInstance;
	}

	return nullptr;
}

void US3DParticleSystemComponent::HandleParticleCollision(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction, FVector Normal, FName BoneName, UPhysicalMaterial* PhysMat)
{
	TArray<FOverlapResult> Overlaps;
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	GetWorld()->OverlapMultiByObjectType(Overlaps, Location, FQuat::Identity, ObjectParams, FCollisionShape::MakeSphere(BulletCollisionSize));

	for(const FOverlapResult& Overlap : Overlaps)
	{
		// Recharge shock cannons if we hit a player projectile
		if(Overlap.GetActor() != nullptr && Overlap.GetActor()->IsA(AS3DPlayerProjectile::StaticClass()))
		{
			AS3DPlayerProjectile* OverlapProjectile = StaticCast<AS3DPlayerProjectile*>(Overlap.GetActor());
			if(OverlapProjectile->DamageType != nullptr)
			{
				AS3DPlayerState* PlayerState = StaticCast<AS3DPlayerState*>(UGameplayStatics::GetPlayerController(this, 0)->PlayerState);
				if(PlayerState != nullptr)
				{
					PlayerState->RechargeShockCannons(OverlapProjectile->DamageType.GetDefaultObject()->ShockCannonChargeOnBulletHit);
				}
			}
		}
	
		UGameplayStatics::ApplyDamage(Overlap.Actor.Get(), 1.0f, nullptr, GetOwner(), BulletDamageType);
	}
}
