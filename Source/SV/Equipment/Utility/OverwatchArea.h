// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Enums/ECharacterEnums.h"
#include "OverwatchArea.generated.h"

UCLASS()
class SV_API AOverwatchArea : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AOverwatchArea();
	void SetOverWatchOwner(AActor* actor);
	FVector GetOverwatchOwnerLocation();

	void SetupForCollision();

protected:

	UFUNCTION() void OnOverlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* MeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) AActor* OverwatchOwner;

	UFUNCTION() void OnCanActivateRequest(ECharacterControl control);
	UFUNCTION() void OnRemoveRequest(ECharacterControl control);

private:

	UPROPERTY() bool bCanActivate;
	UPROPERTY() bool bIsActive;
};
