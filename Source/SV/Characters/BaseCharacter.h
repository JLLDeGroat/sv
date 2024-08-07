// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interfaces/Selectable.h"
#include "../Interfaces/SvChar.h"
#include "BaseCharacter.generated.h"

class ACharacterAIController;
class AAIController;

class UCharacterDetailsComponent;

UCLASS()
class SV_API ABaseCharacter : public ACharacter, public ISelectable, public ISvChar
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

	//Inherited from ISelectable
	virtual FVector GetSelectableGridLocation() override;
	virtual bool TryGetAsMoveable(TScriptInterface<IMovable>& Moveable) override;
	virtual bool IsControlType(ECharacterControl characterControl) override;
	virtual void TryVisualiseTargets() override;
	virtual AActor* GetAsActor() override;

	AAIController* GetAIController() const;

	//inherited from ISVChar
	virtual TArray<TScriptInterface<IHitComponent>> GetHitComponents() override;
	virtual void UpdateActorVisibility(bool value) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UCharacterDetailsComponent* DetailsComponent;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
};
