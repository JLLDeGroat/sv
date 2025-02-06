// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BloodSpatterDecal.generated.h"

class UDecalComponent;

UCLASS()
class SV_API ABloodSpatterDecal : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABloodSpatterDecal();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupForWall();
	void SetupForFloor(FVector originLocation, FRotator rotation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UDecalComponent* Spatter1Component;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UDecalComponent* Spatter2Component;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UDecalComponent* Spatter3Component;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UDecalComponent* Spatter4Component;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UDecalComponent* Spatter5Component;

private:
	UDecalComponent* SetupWallComponent(UDecalComponent* comp);
	UDecalComponent* SetupFloorComponent(FVector originLocation, UDecalComponent* comp);
};
