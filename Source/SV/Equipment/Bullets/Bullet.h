// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class UBulletCollisionComponent;
class UStaticMeshComponent;
class UTravelComponent;
class UBulletDetailsComponent;

UCLASS()
class SV_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* BulletMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UBulletCollisionComponent* BulletCollisionComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UTravelComponent* TravelComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UBulletDetailsComponent* BulletDetailsComponent;


	UFUNCTION() void OnAutoDestroyCallback();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	FTimerHandle AutoDestroyTimer;

};
