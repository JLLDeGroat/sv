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
class AGrenadeIndicatorActor;

struct FInputActionValue;

class ULeftClickAction;
class URightClickAction;
class UTargetAction;
class UGrenadeActionComponent;
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

	void SetMouseAsUi();
	void SetMouseAsGame();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* PlayerInputMappingContext;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* ClickAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* RightClickAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* MoveUpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* MoveRightAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* BeginTargetAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* CameraMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* GrenadeAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) AGridSelectionActor* GridSelection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) AGrenadeIndicatorActor* GrenadeIndicator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) ULeftClickAction* LeftClickActionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) URightClickAction* RightClickActionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTargetAction* TargetActionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UGrenadeActionComponent* GrenadeActionComponent;

	void MoveUp_Started(const FInputActionValue& Value);
	void MoveRight_Started(const FInputActionValue& Value);

	void MouseMove(const FInputActionValue& Value);

	UPROPERTY() float MovementMultiplier;;

private:

	UPROPERTY() USelectionManager* SelectionManager;
	UPROPERTY() UControlManager* ControlManager;
	UPROPERTY() APlayerPawn* PlayerPawn;
};
