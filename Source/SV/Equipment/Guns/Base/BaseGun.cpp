// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseGun.h"
#include "../Components/GunFireComponent.h"
#include "../Components/GunFireSoundComponent.h"

ABaseGun::ABaseGun(const FObjectInitializer &ObjectInitializer)
    : AEquipment(ObjectInitializer)
{
    GunFireComponent = CreateDefaultSubobject<UGunFireComponent>(TEXT("GunFire"));
    GunFireSoundComponent = CreateDefaultSubobject<UGunFireSoundComponent>(TEXT("GunFireSound"));
}

void ABaseGun::OnConstruction(const FTransform &Transform)
{
    Super::OnConstruction(Transform);
}