// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class US3DParticleSystemComponent;

#include "Kismet/BlueprintFunctionLibrary.h"
#include "S3DGameplayStatics.generated.h"

UCLASS()
class SHMUP3D_API US3DGameplayStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Effects|Components|ParticleSystem", meta = (Keywords = "particle system", UnsafeDuringActorConstruction = "true"))
	static US3DParticleSystemComponent* SpawnBulletEmitterAttached(UParticleSystem* EmitterTemplate, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, EAttachLocation::Type LocationType, bool bAutoDestroy);

	UFUNCTION(BlueprintPure, Category = "Input")
	static void GetKeysForAction(APlayerController* PlayerController, FName ActionName, TArray<FInputActionKeyMapping>& Bindings);
};
