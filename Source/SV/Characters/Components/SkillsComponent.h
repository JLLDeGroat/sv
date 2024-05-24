// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Data/SkillData.h"
#include "SkillsComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API USkillsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillsComponent();

	TArray<FSkillDataItem*> GetMeleeSkills();
	void SetCurrentActiveSkill(FSkillDataItem* skill);
	FSkillDataItem* GetCurrentActiveSkill();

	void UseCurrentSkillOnTarget();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY() TArray<FSkillDataItem> Skills;
	UPROPERTY() FSkillDataItem CurrentActiveSkill;
};
