// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StartGameDebugActor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(SVLogStartGameDebugActor, Log, All);

class UStaticMeshComponent;
UCLASS()
class SV_API AStartGameDebugActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStartGameDebugActor();

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "generations")
	bool bShouldStartAtGamePlayMode;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent *RootMesh;

	void StartGamePlayGameMode();
	void StartMenuGameMode();

private:
};
