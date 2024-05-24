// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Components/AttackComponent.h"
#include "../Components/DamageRecieveComponent.h"
#include "../../Interfaces/SvChar.h"
#include "../../Instance/SvGameInstance.h"

// Sets default values for this component's properties
USkillsComponent::USkillsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void USkillsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	auto instance = GetWorld()->GetGameInstance<USvGameInstance>();
	if (instance) {
		FSkillDataItem item;
		instance->GetSkillDataItem("zg_claws", item);
		Skills.Emplace(item);
	}

	for (int i = 0; i < Skills.Num(); i++) 
		UDebugMessages::LogDisplay(this, "found item " + Skills[i].GetName());
}


// Called every frame
void USkillsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FSkillDataItem*> USkillsComponent::GetMeleeSkills() {
	TArray<FSkillDataItem*> items;
	for (int i = 0; i < Skills.Num(); i++) {
		if (Skills[i].GetDamageType() == FSkillData::MeleeDamageType()) 
			items.Emplace(&Skills[i]);
	}
	return items;
}

void USkillsComponent::SetCurrentActiveSkill(FSkillDataItem* skill) {
	CurrentActiveSkill = *skill;
}

FSkillDataItem* USkillsComponent::GetCurrentActiveSkill() {
	return &CurrentActiveSkill;
}

void USkillsComponent::UseCurrentSkillOnTarget() {
	auto attackComponent = GetOwner()->GetComponentByClass<UAttackComponent>();
	if (attackComponent) {
		auto target = attackComponent->GetCurrentTargetCharacter();
		auto damageRecieve = target->GetAsActor()->GetComponentByClass<UDamageRecieveComponent>();

		auto damage = FMath::RandRange(CurrentActiveSkill.GetDamageMin(), CurrentActiveSkill.GetDamageMax());

		damageRecieve->DoDamage(1.0f, damage);
	}
}