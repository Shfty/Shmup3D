// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "S3DBulletRadar.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHMUP3D_API AS3DBulletRadar : public AActor
{
	GENERATED_BODY()

public:	
	AS3DBulletRadar(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(EditAnywhere, Category = "Bullet Radar")
	USceneComponent* RootScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Radar")
	UStaticMeshComponent* BubbleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Radar")
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Radar")
	float VisualizationRange;
};
