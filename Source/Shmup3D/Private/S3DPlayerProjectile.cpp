// Fill out your copyright notice in the Description page of Project Settings.

#include "Shmup3D.h"
#include "S3DPlayerProjectile.h"


AS3DPlayerProjectile::AS3DPlayerProjectile() : 
	DamageType(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}