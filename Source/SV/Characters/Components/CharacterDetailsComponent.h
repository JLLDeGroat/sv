// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Enums/ECharacterEnums.h"
#include "CharacterDetailsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UCharacterDetailsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterDetailsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetCharacterControl(ECharacterControl characterControl);
	ECharacterControl GetCharacterControl() const;

	void SetHealth(int health);
	int GetHealth() const;

	void RemoveHealth(int health, bool& isDead);

private:

	UPROPERTY() ECharacterControl ControlType;
	UPROPERTY() int Health;
		
};
