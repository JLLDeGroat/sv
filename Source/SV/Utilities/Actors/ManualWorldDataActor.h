// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Enums/EWorldEnums.h"
#include "ManualWorldDataActor.generated.h"

UCLASS()
class SV_API AManualWorldDataActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AManualWorldDataActor();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* RootMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) EMissionType MissionType;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GenerateWorldLocationData();
	void GenerateWorldLocationMissionsData();

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bShouldGenerate;

};
