// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBullet.generated.h"

class UBulletCollisionComponent;
class UStaticMeshComponent;
class UTravelComponent;
class UBulletDetailsComponent;
class UBulletTrailComponent;
class UBulletSoundComponent;
class UBulletHitSoundComponent;
class UBulletRearComponent;

UCLASS()
class SV_API ABaseBullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseBullet(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* BulletMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UBulletCollisionComponent* BulletCollisionComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UTravelComponent* TravelComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UBulletDetailsComponent* BulletDetailsComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UBulletTrailComponent* BulletTrailComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UBulletSoundComponent* BulletFireSoundComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UBulletHitSoundComponent* BulletHitSoundComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UBulletRearComponent* BulletRearComponent;


	UFUNCTION() void OnAutoDestroyCallback();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
