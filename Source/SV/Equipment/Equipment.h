// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Equipment.generated.h"

class UEquipmentDetailsComponent;
class UAttachedVectorComponent;

UCLASS()
class SV_API AEquipment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquipment(const FObjectInitializer& ObjectInitializer);

	virtual void SetupAttachVector();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UEquipmentDetailsComponent* EquipmentDetailsComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UAttachedVectorComponent* AttachedVectorComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
