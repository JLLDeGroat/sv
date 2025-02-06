// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../Enums/EEquipmentEnums.h"
#include "GunFireSoundComponent.generated.h"

class USoundWave;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UGunFireSoundComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGunFireSoundComponent();

protected:
	void TestEchoes(FVector echoLoc, USoundWave *sound);

public:
	virtual void BeginPlay() override;

	void MakeGunFireSound(EGun gunType, FVector location);

private:
	static FString RifleGunFireSounds[9];
	static FString PistolGunFireSounds[4];
};
