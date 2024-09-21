// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Enums/EEquipmentEnums.h"
#include "AttachedVectorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UAttachedVectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttachedVectorComponent();

	bool GetAttachmentsForType(EAttachType attachType, FVector& location, FRotator& rotation);

	void SetAttachedVectors(FVector location, FRotator rotation);
	void SetHolsteredVectors(FVector location, FRotator rotation);

	FVector GetAttachedLocation();
	FRotator GetAttachedRotation();

	FVector GetHolsteredLocation();
	FRotator GetHolsteredRotation();

	void UseAttachedVectors();
	void UseHolstedVectors();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:

	UPROPERTY() FVector AttachedLocation;
	UPROPERTY() FRotator AttachedRotation;

	UPROPERTY() FVector HolsteredLocation;
	UPROPERTY() FRotator HolsteredRotation;
};
