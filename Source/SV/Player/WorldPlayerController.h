// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WorldPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class ULeftClickAction;
/**
 * 
 */
UCLASS()
class SV_API AWorldPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AWorldPlayerController();

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* PlayerInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* ClickAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) ULeftClickAction* LeftClickActionComponent;
};
