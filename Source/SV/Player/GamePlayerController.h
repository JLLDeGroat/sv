// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
class AGridSelectionActor;
class USelectionManager;
class UControlManager;
class APlayerPawn;

struct FInputActionValue;
/**
 * 
 */
UCLASS()
class SV_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AGamePlayerController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	USelectionManager* GetSelectionManager();
	UControlManager* GetControlManager();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* PlayerInputMappingContext;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* ClickAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* RightClickAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* MoveUpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* MoveRightAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* BeginTargetAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* CameraMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) AGridSelectionActor* GridSelection;

	void ClickAction_Started();
	void ClickAction_Released();
	void RightClickAction_Started();
	void RightClickAction_Released();

	void BeginTarget_Started();

	void MoveUp_Started(const FInputActionValue& Value);
	void MoveRight_Started(const FInputActionValue& Value);

	void MouseMove(const FInputActionValue& Value);

	UPROPERTY() float MovementMultiplier;

private:

	UPROPERTY() USelectionManager* SelectionManager;
	UPROPERTY() UControlManager* ControlManager;
	UPROPERTY() APlayerPawn* PlayerPawn;
};
