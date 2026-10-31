// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../Enums/EEquipmentEnums.h"
#include "ClothingAttachComponent.generated.h"

class UStaticMeshComponent;

USTRUCT(BlueprintType)
struct SV_API FClothingAttachData
{
	GENERATED_BODY()

public:
	FClothingAttachData()
	{
		AttachMesh = nullptr;
		AttachType = EAttachType::INVALID;
		AttachOffset = FVector::ZeroVector;
		AttachRotationOffset = FRotator::ZeroRotator;
	}
	FClothingAttachData(UStaticMeshComponent *mesh, EAttachType type, FVector offset, FRotator rotationOffset = FRotator::ZeroRotator, FVector scale = FVector(1))
	{
		AttachMesh = mesh;
		AttachType = type;
		AttachOffset = offset;
		AttachRotationOffset = rotationOffset;
		Scale = scale;
	}
	TObjectPtr<UStaticMeshComponent> GetMesh() const { return AttachMesh; }
	EAttachType GetType() const { return AttachType; }
	FVector GetOffset() const { return AttachOffset; }
	FRotator GetRotationOffset() const { return AttachRotationOffset; }
	FVector GetScale() const { return Scale; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing|Attachments")
	TObjectPtr<UStaticMeshComponent> AttachMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing|Attachments")
	EAttachType AttachType = EAttachType::INVALID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing|Attachments")
	FVector AttachOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing|Attachments")
	FRotator AttachRotationOffset = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing|Attachments")
	FVector Scale = FVector(1);
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UClothingAttachComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UClothingAttachComponent();

	void AddToAttachments(UStaticMeshComponent *mesh, EAttachType attachType, FVector offset = FVector::ZeroVector, FRotator rotationOffset = FRotator::ZeroRotator, FVector scale = FVector(1));

	TArray<FClothingAttachData> GetAttachments() const { return Attachments; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing|Attachments")
	TArray<FClothingAttachData> Attachments;
};
