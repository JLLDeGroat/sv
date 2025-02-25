// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Enums/ECharacterEnums.h"
#include "CharacterDetailsComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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

	void RefreshOnNewTurn();

	void SetCharacterControl(ECharacterControl characterControl);
	ECharacterControl GetCharacterControl() const;

	void SetHealth(int health);
	int GetHealth() const;

	void SetMaxHealth(int health);
	int GetMaxHealth() const;

	void RemoveHealth(int health, bool& isDead);
	void AddHealth(int health);

	void RemoveMovementPoints(int amount);
	void AddMovementPoints(int amount);
	int GetMovementPoints() const;

	void RemoveMaxMovementPoints(int amount);
	void AddMaxMovementPoints(int amount);
	int GetMaxMovementPoints() const;

	void RemoveActionPoints(int amount);
	void AddActionPoints(int amount);
	int GetActionPoints() const;

	void RemoveMaxActionPoints(int amount);
	void AddMaxActionPoints(int amount);
	int GetMaxActionPoints() const;

	void SetCanVault(bool value);
	bool GetCanVault();

	void SetCanSkip(bool value);
	bool GetCanSkip();

	void SetCharacterName(FString name);
	FString GetCharacterName();

	float GetHealthAsPercentage() const;

	FGuid GetCharacterId() const;
	void SetCharacterId(FGuid guid);

	bool GetIsDead();

	ETargetIcon GetTargetIcon();
	void SetTargetIcon(ETargetIcon targetIcon);

	void SetBloodType(EBloodType bType);
	EBloodType GetBloodType();
private:

	UPROPERTY() ECharacterControl ControlType;
	UPROPERTY() int Health;
	UPROPERTY() int MaxHealth;
	UPROPERTY() int MovementPoints;
	UPROPERTY() int MaxMovementPoints;
	UPROPERTY() int ActionPoints;
	UPROPERTY() int MaxActionPoints;
	UPROPERTY() bool bCanVault;
	UPROPERTY() bool bCanSkip;
	UPROPERTY() FString CharacterName;
	UPROPERTY() FGuid CharacterId;
	UPROPERTY() ETargetIcon TargetIcon;
	UPROPERTY() EBloodType BloodType;
};
