// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseIndicatorActor.generated.h"

class UStaticMeshComponent;
class UIndicatorActivatorComponent;

UCLASS()
class SV_API ABaseIndicatorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseIndicatorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* BaseMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UIndicatorActivatorComponent* ActivatorIndicator;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UMaterialInstanceDynamic* IndicatorMaterial;


	void SetMaterialColour(FLinearColor color);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
