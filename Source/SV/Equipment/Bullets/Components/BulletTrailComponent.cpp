// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletTrailComponent.h"
#include "../../../Utilities/SvUtilities.h"

UBulletTrailComponent::UBulletTrailComponent() {
	auto basicAssetRef = "/Script/Niagara.NiagaraSystem'/Game/Effects/Trails/BasicBulletTrail_N.BasicBulletTrail_N'";
	auto niagaraSystem = USvUtilities::GetNiagaraSystem(basicAssetRef);
	if (niagaraSystem) {
		SetAsset(niagaraSystem);
	}
}

