// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WorldPawn.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UCameraComponent;
class USpotLightComponent;
class UWorldPawnMovementComponent;

UCLASS()
class SV_API AWorldPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWorldPawn();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	USkeletalMeshComponent* GetSkeleMeshComponent();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(BlueprintReadWrite, EditAnywhere) USkeletalMeshComponent* SkeleMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UCapsuleComponent* CapsuleComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UCameraComponent* Camera;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) USpotLightComponent* SpotLightComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UWorldPawnMovementComponent* PawnMovementComponent;
};
