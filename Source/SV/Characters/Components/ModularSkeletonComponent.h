// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "../../Enums/ECharacterEnums.h"
#include "ModularSkeletonComponent.generated.h"

class UPhysicsField;
class UGeometryCollectionComponent;
class URadialVector;
class URadialFalloff;
class AEquipment;
/**
 *
 */
UCLASS()
class SV_API UModularSkeletonComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:

	UModularSkeletonComponent(const FObjectInitializer& ObjectInitializer);

	void SetModularPartHealth(int amount);
	void RemoveHealth(int amount);
	bool GetIsDead();

	void AddDebuffOnDestroy(EDebuffType debuff, float amount);
	void SetupStaticMeshComp(FString meshref, FVector loc = FVector::ZeroVector, FRotator rot = FRotator::ZeroRotator);
	TMap<EDebuffType, float> GetDebuffsOnDestruction();

	void DestroyModularComponent(FVector direction);

	void AddChildEquipment(AEquipment* equipment);
protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) int Health;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) TMap<EDebuffType, float> DebuffsOnDestroy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UPhysicsFieldComponent* ExplosionField;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UGeometryCollectionComponent* DestructibleMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) URadialVector* RadialVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) URadialFalloff* RadialFallOff;

	UPROPERTY() TArray<AEquipment*> ChildEquipment;
};
