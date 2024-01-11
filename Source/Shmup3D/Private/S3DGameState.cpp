// Fill out your copyright notice in the Description page of Project Settings.

#include "Shmup3D.h"

#include "S3DParticleSystemComponent.h"

#include "S3DGameState.h"

AS3DGameState::AS3DGameState(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}


void AS3DGameState::Tick(float DeltaTime)
{
	BulletLocations.Empty();

	for(US3DParticleSystemComponent* BulletParticleComponent : BulletParticleComponents)
	{
		// Get World and Local bullet emitters
		FParticleEmitterInstance* BulletWorld = BulletParticleComponent->GetEmitterInstanceByName(TEXT("BulletWorld"));
		FParticleEmitterInstance* BulletLocal = BulletParticleComponent->GetEmitterInstanceByName(TEXT("BulletLocal"));
		if(BulletWorld != nullptr && BulletLocal != nullptr)
		{
			for(int i = 0; i < BulletWorld->ActiveParticles; ++i)
			{
				FBaseParticle* Particle = BulletWorld->GetParticle(i);
				BulletLocations.Add(Particle->Location);
			}

			for(int i = 0; i < BulletLocal->ActiveParticles; ++i)
			{
				FBaseParticle* Particle = BulletLocal->GetParticle(i);
				FVector TransformedLocation = PlayVolume->GetTransform().TransformPosition(Particle->Location);
				BulletLocations.Add(TransformedLocation);
			}
		}
	}

	Super::Tick(DeltaTime);
}

US3DParticleSystemComponent* AS3DGameState::GetBulletParticleComponent(UParticleSystem* Template,
																	   UMaterialInterface* Material,
																	   bool bUseLocalSpace,
																	   float BulletLifetime,
																	   float BulletSize,
																	   float BulletCollisionSize)
{
	US3DParticleSystemComponent* BulletParticleComponent = nullptr;

	US3DParticleSystemComponent** FoundItem = BulletParticleComponents.FindByPredicate([&](US3DParticleSystemComponent* CandidateBulletParticleComponent) {
		return CandidateBulletParticleComponent->Template == Template
			&& CandidateBulletParticleComponent->BulletMaterial == Material
			&& CandidateBulletParticleComponent->bUseLocalSpace == bUseLocalSpace
			&& CandidateBulletParticleComponent->BulletLifetime == BulletLifetime
			&& CandidateBulletParticleComponent->BulletSize == BulletSize
			&& CandidateBulletParticleComponent->BulletCollisionSize == BulletCollisionSize;
	});

	if(FoundItem != nullptr)
	{
		BulletParticleComponent = *FoundItem;
	}
	else
	{
		if(PlayVolume != nullptr)
		{
			BulletParticleComponent = NewObject<US3DParticleSystemComponent>(PlayVolume);
			BulletParticleComponent->Template = Template;
			BulletParticleComponent->BulletMaterial = Material;
			BulletParticleComponent->bUseLocalSpace = bUseLocalSpace;
			BulletParticleComponent->BulletLifetime = BulletLifetime;
			BulletParticleComponent->BulletSize = BulletSize;
			BulletParticleComponent->BulletCollisionSize = BulletCollisionSize;
			BulletParticleComponent->SetupAttachment(PlayVolume->GetRootComponent());
			BulletParticleComponent->RegisterComponent();
			BulletParticleComponents.Add(BulletParticleComponent);

			OnBulletParticleComponentAdded.Broadcast(BulletParticleComponent);
		}
		else
		{
			UE_LOG(Shmup3D, Warning, TEXT("Failed to create bullet particle component."))
		}
	}

	return BulletParticleComponent;
}
