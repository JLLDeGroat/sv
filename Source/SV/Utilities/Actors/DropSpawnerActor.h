// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Enums/EEquipmentEnums.h"
#include "GameFramework/Actor.h"
#include "DropSpawnerActor.generated.h"

UCLASS()
class SV_API ADropSpawnerActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADropSpawnerActor();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* RootMesh;

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) EResourceType ResType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int Amount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bShouldGenerate;

};
