// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Base/BaseRunnable.h"
#include "BaseAITurnManager.generated.h"

class ISvChar;
class UBaseAIBehaviour;
/**
 *
 */
UCLASS()
class SV_API UBaseAITurnManager : public UBaseRunnable
{
	GENERATED_BODY()

public:

	UBaseAITurnManager(const FObjectInitializer& ObjectInitializer);

	void SetupTurnManager(UWorld* world);
	void SetThisEnemy(TScriptInterface<ISvChar> enemy);
	void SetAllCharacters(TArray<TScriptInterface<ISvChar>> characters);

	virtual void ActivateThread() override;



	bool GetCheckerHasCompleted() const;
	bool GetCheckerHasCompletedAndWaitIfNot(int seconds) const;
	UFUNCTION() void SetCheckerHasCompleted();

	UFUNCTION() void SetHasFinishedTurnEarly();
	bool GetHasFinishedTurnEarly();


	bool GetThisEnemyIsValidAndAlive();
protected:

	UWorld* GetWorld() const;

	TScriptInterface<ISvChar> GetThisEnemy();
	TArray<TScriptInterface<ISvChar>> GetAllCharacters();

	UBaseAIBehaviour* CreateBehaviourClass(UClass* cls);

private:

	UPROPERTY() TArray<TScriptInterface<ISvChar>> AllCharacters;
	UPROPERTY() TScriptInterface<ISvChar> ThisEnemy;

	UPROPERTY() bool bCheckerIsComplete;
	UPROPERTY() bool HasFinishedTurnEarly;

};
