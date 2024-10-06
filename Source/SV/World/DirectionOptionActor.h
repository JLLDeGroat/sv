// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DirectionOptionActor.generated.h"

class UDecalComponent;
class UBoxComponent;
class UMaterialInstanceDynamic;
UCLASS()
class SV_API ADirectionOptionActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADirectionOptionActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ActivateArrowEmission();
	void DeactivateArrowEmission();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UDecalComponent* ArrowDecalComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UBoxComponent* ClickBoxComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UMaterialInstanceDynamic* DynamicInstance;

};
