// Fill out your copyright notice in the Description page of Project Settings.

#include "Shmup3D.h"

#include "S3DGameState.h"

#include "S3DBulletRadar.h"


AS3DBulletRadar::AS3DBulletRadar(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	Range(2000.0f),
	VisualizationRange(200.0f)
{
	RootScene = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("RootScene"));
	RootComponent = RootScene;

	// Setup sphere mesh
	BubbleMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("BubbleMesh"));
	BubbleMesh->SetupAttachment(RootComponent);
	BubbleMesh->SetCollisionProfileName(TEXT("NoCollision"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> EngineSphere(TEXT("/Engine/ArtTools/RenderToTexture/Meshes/S_1_Unit_Sphere_HI"));
	if(EngineSphere.Succeeded())
	{
		BubbleMesh->SetStaticMesh(EngineSphere.Object);
	}

	PrimaryActorTick.bCanEverTick = true;
}

