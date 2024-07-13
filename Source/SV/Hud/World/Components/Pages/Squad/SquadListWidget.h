// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SquadListWidget.generated.h"

/**
 * 
 */
UCLASS()
class SV_API USquadListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void SetupCrewList();
	virtual void NativeConstruct() override;

protected:

	UFUNCTION() void Crew1Clicked();
	UFUNCTION() void Crew2Clicked();
	UFUNCTION() void Crew3Clicked();
	UFUNCTION() void Crew4Clicked();
	UFUNCTION() void Crew5Clicked();
	UFUNCTION() void Crew6Clicked();

private:

	void ShowCrewDetails(int crewClicked);

};
