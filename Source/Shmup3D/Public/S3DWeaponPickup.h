// © 2016 Josh Palmer

#pragma once

class AS3DPlayerWeapon;

#include "S3DPickup.h"
#include "S3DWeaponPickup.generated.h"

UCLASS()
class SHMUP3D_API AS3DWeaponPickup : public AS3DPickup
{
	GENERATED_BODY()

	AS3DWeaponPickup(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UDecalComponent* IconDecal;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon Pickup")
	TSubclassOf<AS3DPlayerWeapon> WeaponClass;

private:
	UPROPERTY()
	UMaterialInstanceDynamic* DecalDynamicMaterial;
};
