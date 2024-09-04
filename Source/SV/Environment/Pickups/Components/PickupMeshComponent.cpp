// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../../../Utilities/SvUtilities.h"

UPickupMeshComponent::UPickupMeshComponent() {

	SetCollisionResponseToAllChannels(ECR_Overlap);
	SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
}

void UPickupMeshComponent::SetMesh(FString mesh) {
	auto meshObj = USvUtilities::GetStaticMesh(mesh);
	if (meshObj)
		SetStaticMesh(meshObj);

}