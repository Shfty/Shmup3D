// © 2016 Josh Palmer

#pragma once

#include "GameFramework/Actor.h"
#include "S3DPickup.generated.h"

UCLASS()
class SHMUP3D_API AS3DPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AS3DPickup(const FObjectInitializer& ObjectInitializer);
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
protected:
	UFUNCTION()
	void HandleSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintNativeEvent)
	void OnPickedUp();

protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* RootSceneComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereCollisionComponent;

	UPROPERTY(EditAnywhere)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats")
	UParticleSystem* PickedUpEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats")
	USoundCue* PickedUpSound;
};
