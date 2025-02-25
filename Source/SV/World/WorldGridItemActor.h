// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldGridItemActor.generated.h"

UCLASS()
class SV_API AWorldGridItemActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWorldGridItemActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetIsStart();
	void SetIsEnd();
	void SetIsOffshoot();
	void SetIsSpawn();
	void SetIsObstacle();
	void SetIsWall();
	void SetIsDebris();

	void SetAutoDestroy(float timer = 1.0f);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* MeshComponent;

	UFUNCTION() void OnAutoDestroy();
	FTimerHandle AutoDestroyHandle;

private:

	UPROPERTY() bool bIsStart;
	UPROPERTY() bool bIsEnd;
	UPROPERTY() bool bIsOffshoot;

};
