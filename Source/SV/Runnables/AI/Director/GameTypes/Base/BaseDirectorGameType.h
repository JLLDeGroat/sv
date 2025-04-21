// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseDirectorGameType.generated.h"

class ISvChar;
class ABaseCharacter;
/**
 *
 */
UCLASS()
class SV_API UBaseDirectorGameType : public UObject
{
    GENERATED_BODY()

public:
    UBaseDirectorGameType(const FObjectInitializer &ObjectInitializer);

    virtual float GeneratePreassure();
    virtual void SpawnRequiredEnemies();

protected:
    void MoveThisCharTowardsPlayer(TScriptInterface<ISvChar> movableCharacter);
    void SpawnEnemyOfTypeAtLocation(ABaseCharacter *baseCharacter, FVector loc, bool spawnFromFloor = false);

    bool IsWithinList(FVector loc, TArray<FVector> location);

    UPROPERTY()
    FRandomStream _randomStream;

    void SetPlayerAndEnemyCharacters();
    UPROPERTY()
    TArray<TScriptInterface<ISvChar>> PlayerCharacters;
    UPROPERTY()
    TArray<TScriptInterface<ISvChar>> EnemyCharacters;

private:
};
