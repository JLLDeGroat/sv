// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SquadListWidget.generated.h"

class UButton;
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

	UPROPERTY(meta = (BindWidget)) UButton* CrewBtn1;
	UPROPERTY(meta = (BindWidget)) UButton* CrewBtn2;
	UPROPERTY(meta = (BindWidget)) UButton* CrewBtn3;
	UPROPERTY(meta = (BindWidget)) UButton* CrewBtn4;
	UPROPERTY(meta = (BindWidget)) UButton* CrewBtn5;
	UPROPERTY(meta = (BindWidget)) UButton* CrewBtn6;

private:

	void ShowCrewDetails(int crewClicked);

};
