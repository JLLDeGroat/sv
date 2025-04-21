// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DirectorDelegates.generated.h"

/**
 *
 */
UCLASS()
class SV_API UDirectorDelegates : public UObject
{
	GENERATED_BODY()
public:
	static UDirectorDelegates *GetInstance()
	{
		static UDirectorDelegates *directorDelegates = 0;
		if (!directorDelegates)
		{
			directorDelegates = NewObject<UDirectorDelegates>();
			directorDelegates->AddToRoot();
		}
		return directorDelegates;
	}
};
