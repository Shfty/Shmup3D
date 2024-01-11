// Fill out your copyright notice in the Description page of Project Settings.

#include "Shmup3D.h"
#include "S3DGameInstance.h"

US3DGameInstance::US3DGameInstance(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

void US3DGameInstance::Init()
{
	InputProcessor = MakeShareable(new FSPInputProcessor());
	InputProcessor->OnActiveInputDeviceChanged.BindUObject(this, &US3DGameInstance::ActiveInputDeviceChanged);
	FSlateApplication::Get().SetInputPreProcessor(true, InputProcessor);

	Super::Init();
}

EActiveInputDevice US3DGameInstance::GetActiveInputDevice()
{
	return InputProcessor->ActiveInputDevice;
}

void US3DGameInstance::ActiveInputDeviceChanged(EActiveInputDevice ActiveInputDevice)
{
	OnActiveInputDeviceChanged.Broadcast(ActiveInputDevice);
}
