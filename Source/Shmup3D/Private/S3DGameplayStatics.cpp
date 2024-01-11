// Fill out your copyright notice in the Description page of Project Settings.

#include "Shmup3D.h"

#include "S3DParticleSystemComponent.h"

#include "S3DGameplayStatics.h"

US3DParticleSystemComponent* CreateBulletParticleSystem(UParticleSystem* EmitterTemplate, UWorld* World, AActor* Actor, bool bAutoDestroy)
{
	US3DParticleSystemComponent* PSC = NewObject<US3DParticleSystemComponent>((Actor ? Actor : (UObject*)World));
	PSC->bAutoDestroy = bAutoDestroy;
	PSC->bAllowAnyoneToDestroyMe = true;
	PSC->SecondsBeforeInactive = 0.0f;
	PSC->bAutoActivate = false;
	PSC->SetTemplate(EmitterTemplate);
	PSC->bOverrideLODMethod = false;

	return PSC;
}

US3DParticleSystemComponent* US3DGameplayStatics::SpawnBulletEmitterAttached(UParticleSystem* EmitterTemplate, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, EAttachLocation::Type LocationType, bool bAutoDestroy)
{
	US3DParticleSystemComponent* PSC = nullptr;
	if(EmitterTemplate)
	{
		if(AttachToComponent == nullptr)
		{
			UE_LOG(LogScript, Warning, TEXT("UGameplayStatics::SpawnEmitterAttached: NULL AttachComponent specified!"));
		}
		else
		{
			UWorld* const World = AttachToComponent->GetWorld();
			if(World && World->GetNetMode() != NM_DedicatedServer)
			{
				PSC = CreateBulletParticleSystem(EmitterTemplate, World, AttachToComponent->GetOwner(), bAutoDestroy);

				PSC->SetupAttachment(AttachToComponent, AttachPointName);

				if(LocationType == EAttachLocation::KeepWorldPosition)
				{
					const FTransform ParentToWorld = AttachToComponent->GetSocketTransform(AttachPointName);
					const FTransform ComponentToWorld(Rotation, Location);
					const FTransform RelativeTM = ComponentToWorld.GetRelativeTransform(ParentToWorld);
					PSC->RelativeLocation = RelativeTM.GetLocation();
					PSC->RelativeRotation = RelativeTM.GetRotation().Rotator();
				}
				else
				{
					PSC->RelativeLocation = Location;
					PSC->RelativeRotation = Rotation;
				}
				PSC->RelativeScale3D = FVector(1.f);

				PSC->RegisterComponentWithWorld(World);
				PSC->ActivateSystem(true);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
				if(PSC->Template && PSC->Template->IsImmortal())
				{
					UE_LOG(LogTemp, Log, TEXT("GameplayStatics::SpawnEmitterAttached spawned potentially immortal particle system! %s (%s) may stay in world despite never spawning particles after burst spawning is over."),
						   *(PSC->GetPathName()), *(PSC->Template->GetPathName())
					);
				}
#endif
			}
		}
	}
	return PSC;
}

void US3DGameplayStatics::GetKeysForAction(APlayerController* PlayerController, FName ActionName, TArray<FInputActionKeyMapping>& Bindings)
{
	Bindings = PlayerController->PlayerInput->GetKeysForAction(ActionName);
}
