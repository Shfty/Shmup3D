// © 2016 Josh Palmer

#include "Shmup3D.h"

#include "S3DPickup.h"

#include "S3DPlayerPawn.h"


// Sets default values
AS3DPickup::AS3DPickup(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	RootSceneComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("RootScene"));
	RootComponent = RootSceneComponent;

	MeshComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(RootSceneComponent);

	SphereCollisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Sphere"));
	SphereCollisionComponent->SetCollisionProfileName(TEXT("Pickup"));
	SphereCollisionComponent->SetSphereRadius(128.0f, false);
	SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AS3DPickup::HandleSphereOverlap);
	SphereCollisionComponent->SetupAttachment(RootSceneComponent);
}

void AS3DPickup::Tick(float DeltaSeconds)
{
	MeshComponent->AddRelativeRotation(FRotator(0.0f, 180.0f * DeltaSeconds, 0.0f));

	Super::Tick(DeltaSeconds);
}

void AS3DPickup::OnConstruction(const FTransform& Transform)
{
	MeshComponent->SetStaticMesh(StaticMesh);
	MeshComponent->SetMaterial(0, Material);

	Super::OnConstruction(Transform);
}

void AS3DPickup::HandleSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(OtherActor->IsA(AS3DPlayerPawn::StaticClass()))
	{
		OnPickedUp();
	}
}

void AS3DPickup::OnPickedUp_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickedUpEffect, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickedUpSound, GetActorLocation());
	Destroy();
}
