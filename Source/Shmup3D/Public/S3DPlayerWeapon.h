// © 2016 Josh Palmer

#pragma once

#include "GameFramework/Actor.h"
#include "S3DPlayerWeapon.generated.h"

UCLASS()
class SHMUP3D_API AS3DPlayerWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AS3DPlayerWeapon();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Weapon")
	UTexture2D* WeaponIcon;
};
