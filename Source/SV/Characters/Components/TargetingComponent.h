// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"

class ISvChar;

USTRUCT()
struct FTargetData
{
	GENERATED_BODY()

public:

	FTargetData() {

	}

	FTargetData(FVector shootFromLoc, TScriptInterface<ISvChar> svChar) {
		ShootFromLocation = shootFromLoc;
		Character = svChar;
	}

	TScriptInterface<ISvChar> GetCharacter() { return Character; }
	FVector GetShootLocation() const { return ShootFromLocation; }

protected:

	UPROPERTY() FVector ShootFromLocation;
	UPROPERTY() TScriptInterface<ISvChar> Character;

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTargetingComponent();

	void DetermineTargetData();

	TArray<FTargetData> GetCurrentTargetData();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY() TArray<FTargetData> TargetData;

	bool ObtainPotentialTargetList(TArray<TScriptInterface<ISvChar>>& FoundCharacters) const;
	TArray<FVector> GetPotentialShootingLocations();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
