// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class UCameraComponent;
class UPawnCameraComponent;
class UPostProcessComponent;
class UCameraOverlapComponent;

UCLASS()
class SV_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

	UPawnCameraComponent* GetPawnCameraComponent();
	UCameraComponent* GetCameraComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UCameraComponent* Camera;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UPawnCameraComponent* CameraComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UCameraOverlapComponent* CameraOverlapComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UPostProcessComponent* PostProcessComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

};
