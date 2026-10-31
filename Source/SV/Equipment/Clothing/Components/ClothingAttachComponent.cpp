// Fill out your copyright notice in the Description page of Project Settings.

#include "ClothingAttachComponent.h"

// Sets default values for this component's properties
UClothingAttachComponent::UClothingAttachComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UClothingAttachComponent::AddToAttachments(UStaticMeshComponent *mesh, EAttachType attachType, FVector vectorOffset, FRotator rotationOffset, FVector scale)
{
	FClothingAttachData newAttachment = FClothingAttachData(mesh, attachType, vectorOffset, rotationOffset, scale);
	Attachments.Add(newAttachment);
}