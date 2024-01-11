// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"

#define COLLISION_PLAYVOLUME	ECC_GameTraceChannel1
#define COLLISION_PLAYBOUNDS	ECC_GameTraceChannel2
#define COLLISION_ENEMY			ECC_GameTraceChannel3
#define COLLISION_PLAYERBOUNDS	ECC_GameTraceChannel4
#define COLLISION_BULLETRADAR	ECC_GameTraceChannel5
#define COLLISION_TRIGGER		ECC_GameTraceChannel6
#define COLLISION_PICKUP		ECC_GameTraceChannel7

DECLARE_LOG_CATEGORY_EXTERN(Shmup3D, All, All);