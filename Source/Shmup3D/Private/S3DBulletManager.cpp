// Fill out your copyright notice in the Description page of Project Settings.

#include "Shmup3D.h"
#include "S3DBulletManager.h"


// Sets default values
AS3DBulletManager::AS3DBulletManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	RootSceneComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("RootScene"));
	RootComponent = RootSceneComponent;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AS3DBulletManager::Tick(float DeltaSeconds)
{
	for(TPair<TSubclassOf<US3DBulletTemplate>, US3DBulletTemplate*> TemplateClass : BulletTemplates)
	{
		US3DBulletTemplate* Template = TemplateClass.Value;

		for(int32 i = 0; i < Template->Instances.Num(); ++i)
		{
			US3DBulletInstance* Instance = Template->Instances[i];

			FVector PreviousLocation = Instance->Transform.GetLocation();
			uint32 InstanceIndex = Template->Instances.IndexOfByKey(Instance);

			MoveBullet(Instance);

			FHitResult HitResult;
			if(CheckBulletCollision(HitResult, Instance, PreviousLocation))
			{
				if(HitResult.Actor.IsValid())
				{
					UGameplayStatics::ApplyDamage(HitResult.Actor.Get(), 1.0f, nullptr, nullptr, nullptr);
				}

				/*
				if(Instance->bIsInWorldSpace)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Template->DeathParticleSystem, FTransform(Instance->Transform.GetLocation()));
				}
				else
				{
					FTransform Transform = Instance->Transform * GetActorTransform();
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Template->DeathParticleSystem, Transform.GetLocation());
				}
				*/

				// Return instance to pool
				Template->Instances.RemoveSwap(Instance);
				InstancePool.Add(Instance);

				// Remove instance mesh
				Template->InstancedStaticMeshComponent->RemoveInstance(InstanceIndex);

				// Decrement loop counter to account for removed element
				--i;
			}
			else
			{
				// Update instance mesh
				Template->InstancedStaticMeshComponent->UpdateInstanceTransform(InstanceIndex,
																				Instance->Transform,
																				Instance->bIsInWorldSpace,
																				InstanceIndex == Template->Instances.Num() - 1);
			}
		}
	}

	Super::Tick(DeltaSeconds);
}

void AS3DBulletManager::BeginDestroy()
{
	for(TPair<TSubclassOf<US3DBulletTemplate>, US3DBulletTemplate*> TemplatePair : BulletTemplates)
	{
		US3DBulletTemplate* Template = TemplatePair.Value;
		for(US3DBulletInstance* Instance : Template->Instances)
		{
			if(Instance != nullptr && !Instance->IsPendingKill())
			{
				Instance->ConditionalBeginDestroy();
			}
		}
		Template->Instances.Empty();

		if(Template != nullptr && !Template->IsPendingKill())
		{
			Template->InstancedStaticMeshComponent->ConditionalBeginDestroy();
			Template = nullptr;
		}

		if(TemplatePair.Value != nullptr && !TemplatePair.Value->IsPendingKill())
		{
			TemplatePair.Value->ConditionalBeginDestroy();
			TemplatePair.Value = nullptr;
		}
	}

	for(US3DBulletInstance* BulletInstance : InstancePool)
	{
		BulletInstance->ConditionalBeginDestroy();
	}
	InstancePool.Empty();

	Super::BeginDestroy();
}

US3DBulletTemplate* AS3DBulletManager::SetupTemplate(TSubclassOf<US3DBulletTemplate> TemplateClass)
{
	if(TemplateClass != nullptr)
	{
		US3DBulletTemplate* Template = NewObject<US3DBulletTemplate>(this, TemplateClass);
		BulletTemplates.Add(TemplateClass, Template);

		UInstancedStaticMeshComponent* InstancedStaticMesh = NewObject<UInstancedStaticMeshComponent>(this);

		InstancedStaticMesh->SetStaticMesh(Template->Mesh);
		InstancedStaticMesh->SetMaterial(0, Template->Material);

		InstancedStaticMesh->SetupAttachment(RootComponent);
		InstancedStaticMesh->RegisterComponent();

		Template->InstancedStaticMeshComponent = InstancedStaticMesh;

		return Template;
	}

	return nullptr;
}

void AS3DBulletManager::SpawnBullet(TSubclassOf<US3DBulletTemplate> TemplateClass, const FTransform& Transform, const FVector& Velocity, const float Size, const float CollisionRadius, const bool bIsInWorldSpace)
{
	if(TemplateClass != nullptr)
	{
		US3DBulletTemplate* Template = BulletTemplates.FindRef(TemplateClass);
		if(Template == nullptr)
		{
			Template = SetupTemplate(TemplateClass);
		}

		FTransform FinalTransform;
		FVector FinalVelocity;
		if(bIsInWorldSpace)
		{
			FinalTransform = Transform;
			FinalVelocity = FinalTransform.TransformVectorNoScale(Velocity);
		}
		else
		{
			FinalTransform = Transform.GetRelativeTransform(GetActorTransform());
			FinalVelocity = FinalTransform.TransformVectorNoScale(Velocity);
		}

		US3DBulletInstance* BulletInstance;
		if(InstancePool.Num() == 0)
		{
			BulletInstance = NewObject<US3DBulletInstance>(this);
		}
		else
		{
			BulletInstance = InstancePool[0];
			InstancePool.RemoveAtSwap(0);
		}
		BulletInstance->Transform = FTransform(FinalTransform.GetRotation(), FinalTransform.GetLocation(), FVector(Size * 0.5f));
		BulletInstance->Velocity = FinalVelocity;
		BulletInstance->CollisionRadius = CollisionRadius * 0.5f;
		BulletInstance->bIsInWorldSpace = bIsInWorldSpace;
		BulletTemplates.FindRef(TemplateClass)->Instances.Add(BulletInstance);

		if(bIsInWorldSpace)
		{
			Template->InstancedStaticMeshComponent->AddInstanceWorldSpace(FinalTransform);
		}
		else
		{
			Template->InstancedStaticMeshComponent->AddInstance(FinalTransform);
		}
	}
}

void AS3DBulletManager::MoveBullet(US3DBulletInstance* Instance)
{
	Instance->Transform.SetLocation(Instance->Transform.GetLocation() + Instance->Velocity * GetWorld()->GetDeltaSeconds());
}

bool AS3DBulletManager::CheckBulletCollision(FHitResult& HitResult, US3DBulletInstance* Instance, const FVector& PreviousLocation)
{
	FVector Start;
	FVector End;

	FVector BoxIntersection;
	FBox Bounds;

	if(Instance->bIsInWorldSpace)
	{
		Start = PreviousLocation;
		End = Instance->Transform.GetLocation();
		BoxIntersection = End;
		Bounds = FBox::BuildAABB(GetActorLocation(), FVector(3000.0f));
	}
	else
	{
		Start = GetActorLocation() + PreviousLocation;
		End = GetActorLocation() + Instance->Transform.GetLocation();
		BoxIntersection = GetActorRotation().UnrotateVector(Instance->Transform.GetLocation());
		Bounds = FBox::BuildAABB(FVector::ZeroVector, FVector(3000.0f));
	}

	// Bounds check for early-out
	if(!Bounds.IsInsideOrOn(BoxIntersection))
	{
		return true;
	}

	// Swept sphere intersection
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	ObjectQueryParams.AddObjectTypesToQuery(COLLISION_PLAYBOUNDS);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	return GetWorld()->SweepSingleByObjectType(HitResult,
											   Start,
											   End,
											   FQuat::Identity, ObjectQueryParams,
											   FCollisionShape::MakeSphere(Instance->CollisionRadius),
											   QueryParams);
}
