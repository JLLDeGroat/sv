// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseBullet.h"
#include "Bullet.generated.h"


UCLASS()
class SV_API ABullet : public ABaseBullet
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullet(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	FTimerHandle AutoDestroyTimer;

};
