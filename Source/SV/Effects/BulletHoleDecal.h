// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletHoleDecal.generated.h"

class UNiagaraComponent;
class UDecalComponent;
/**
 *
 */
UCLASS()
class SV_API ABulletHoleDecal : public AActor
{
	GENERATED_BODY()

public:

	ABulletHoleDecal(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) USceneComponent* SceneRootComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UDecalComponent* DecalComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UNiagaraComponent* BulletShavingsComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UMaterialInstanceDynamic* DecalDynamicMaterial;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float EmissionDecayValue;

	UFUNCTION() void DestroyCallback();
	FTimerHandle DestroyHandle;

};
