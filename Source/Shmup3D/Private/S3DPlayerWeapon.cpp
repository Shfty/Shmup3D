// © 2016 Josh Palmer

#include "Shmup3D.h"
#include "S3DPlayerWeapon.h"


// Sets default values
AS3DPlayerWeapon::AS3DPlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AS3DPlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AS3DPlayerWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

