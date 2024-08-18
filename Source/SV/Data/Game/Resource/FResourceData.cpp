// Fill out your copyright notice in the Description page of Project Settings.


#include "FResourceData.h"

FResourceData::FResourceData()
{
}

FResourceItemData* FResourceData::GetResource(EResourceType resourceType) {
	for (int i = 0; i < ResourceItems.Num(); i++)
		if (ResourceItems[i].GetResourceType() == resourceType)
			return &ResourceItems[i];

	auto newResource = FResourceItemData(resourceType);
	ResourceItems.Emplace(newResource);
	return GetResource(resourceType);
}