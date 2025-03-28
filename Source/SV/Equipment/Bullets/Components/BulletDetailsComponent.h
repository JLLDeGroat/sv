// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BulletDetailsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UBulletDetailsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBulletDetailsComponent();

	int GetBaseDamage();
	void SetBaseDamage(int damage);

	int GetBaseImpulse();
	void SetBaseImpulse(int impulse);

	void SetGunShotFrom(AActor* gun);
	AActor* GetGunShotFrom();

	int GetPenetrationAbility();
	void SetPenetrationAbility(int ability);

	void RemoveFromPenetrationAbility(int amount);

	void CompleteSetup();
	bool GetIsSetup();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:

	UPROPERTY() int BaseDamage;
	UPROPERTY() int BaseImpulse;

	UPROPERTY() AActor* GunShotFrom;

	UPROPERTY() int PenetrationAbility;
	UPROPERTY() bool bSetup;
		
};
