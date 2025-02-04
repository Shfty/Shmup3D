// © 2016 Josh Palmer

#include "Shmup3D.h"

#include "S3DWeaponPickup.h"

#include "S3DPlayerWeapon.h"

AS3DWeaponPickup::AS3DWeaponPickup(const FObjectInitializer& ObjectInitializer) :
	WeaponClass(nullptr),
	DecalDynamicMaterial(nullptr),
	Super(ObjectInitializer)
{
	IconDecal = ObjectInitializer.CreateDefaultSubobject<UDecalComponent>(this, TEXT("DecalComponent"));
	IconDecal->SetupAttachment(MeshComponent);

	ConstructorHelpers::FObjectFinder<UMaterialInterface> DecalMaterialFinder(TEXT("Material'/Game/Materials/M_AdditiveDecal.M_AdditiveDecal'"));
	if(DecalMaterialFinder.Succeeded())
	{
		IconDecal->SetMaterial(0, DecalMaterialFinder.Object);
	}
}

void AS3DWeaponPickup::BeginPlay()
{
	if(WeaponClass != nullptr)
	{
		DecalDynamicMaterial = IconDecal->CreateDynamicMaterialInstance();
		UTexture2D* IconTexture = WeaponClass->GetDefaultObject<AS3DPlayerWeapon>()->WeaponIcon;
		DecalDynamicMaterial->SetTextureParameterValue(TEXT("DecalTexture"), IconTexture);
		IconDecal->SetDecalMaterial(DecalDynamicMaterial);
	}

	Super::BeginPlay();
}
