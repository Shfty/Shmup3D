// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SPInputProcessor.h"

#include "Engine/GameInstance.h"
#include "S3DGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActiveInputDeviceChangedDelegate, EActiveInputDevice, ActiveInputDevice);

UCLASS()
class SHMUP3D_API US3DGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	US3DGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(BlueprintPure, Category = "Input")
	EActiveInputDevice GetActiveInputDevice();

private:
	UFUNCTION()
	void ActiveInputDeviceChanged(EActiveInputDevice ActiveInputDevice);

public:
	UPROPERTY(BlueprintAssignable, Category = "Input")
	FActiveInputDeviceChangedDelegate OnActiveInputDeviceChanged;

private:
	TSharedPtr<FSPInputProcessor> InputProcessor;
};
