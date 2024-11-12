// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMeleeAttack.h"

#include "../../../Characters/Components/EquipmentComponent.h"
#include "../../../Characters/Components/CharacterDetailsComponent.h"
#include "../../../Characters/Components/SkillsComponent.h"
#include "../../../Characters/Components/AttackComponent.h"

#include "../../../Equipment/Equipment.h"
#include "../../../Equipment/Components/EquipmentDetailsComponent.h"

#include "VgCore/Domain/Debug/DebugMessages.h"

#include "../../../Utilities/GridUtilities.h"
#include "../../../Utilities/SvUtilities.h"
#include "../../../Interfaces/SvChar.h"

UAIMeleeAttack::UAIMeleeAttack(const FObjectInitializer& ObjectInitializer)
	: UBaseAIBehaviour(ObjectInitializer) {
}

void UAIMeleeAttack::DoBehaviour() {
	if (!CanMeleeAnyone()) {
		UDebugMessages::LogWarning(this, "cannot melee anyone, not in range");
		SetCompletedBehaviour();
		return;
	}

	auto equipmentComponent = GetThisEnemy()->GetComponentByClass<UEquipmentComponent>();
	auto currentAttackerDetails = GetThisEnemy()->GetComponentByClass<UCharacterDetailsComponent>();

	if (!equipmentComponent || !currentAttackerDetails) return
		UDebugMessages::LogError(this, "could not find equipmentComponent of currentAttack details of ai/character, will stop trying to melee");

	AEquipment* bestEquipment = nullptr;
	int bestEquipmentAttack = 0;
	if (equipmentComponent) {
		TArray<AEquipment*> meleeEquipment = equipmentComponent->GetAllMeleeEquipment();

		for (int i = 0; i < meleeEquipment.Num(); i++) {
			auto equipmentDetails = meleeEquipment[i]->GetComponentByClass<UEquipmentDetailsComponent>();
			if (!equipmentDetails) {
				UDebugMessages::LogError(this, "no equipment details found for " + meleeEquipment[i]->GetName());
				continue;
			}

			if (!bestEquipment && currentAttackerDetails->GetActionPoints() >= equipmentDetails->GetApCost()) {
				bestEquipment = meleeEquipment[i];
				bestEquipmentAttack = FMath::RandRange(equipmentDetails->GetMinBaseDamage(), equipmentDetails->GetMaxBaseDamage());
				continue;
			}

			if (bestEquipment && currentAttackerDetails->GetActionPoints() >= equipmentDetails->GetApCost()) {
				if (equipmentDetails->GetMaxBaseDamage() > bestEquipmentAttack) {
					bestEquipment = meleeEquipment[i];
					bestEquipmentAttack = FMath::RandRange(equipmentDetails->GetMinBaseDamage(), equipmentDetails->GetMaxBaseDamage());
					continue;
				}
			}
		}
	}

	int bestSkillAttack = 0;
	FSkillDataItem* bestSkill = nullptr;
	auto skillComponent = GetThisEnemy()->GetComponentByClass<USkillsComponent>();
	if (skillComponent) {
		auto skills = skillComponent->GetMeleeSkills();

		for (int i = 0; i < skills.Num(); i++) {
			if (!bestSkill && currentAttackerDetails->GetActionPoints() >= skills[i]->GetApCost()) {
				bestSkillAttack = skills[i]->GetDamageMin();
				bestSkill = skills[i];
				continue;
			}

			if (!bestSkill && currentAttackerDetails->GetActionPoints() >= skills[i]->GetApCost()) {
				bestSkill = skills[i];
				bestSkillAttack = skills[i]->GetDamageMin();
				continue;
			}

			if (bestSkill && currentAttackerDetails->GetActionPoints() >= skills[i]->GetApCost()) {
				if (skills[i]->GetDamageMin() > bestSkillAttack) {
					bestSkill = skills[i];
					bestSkillAttack = skills[i]->GetDamageMin();
					continue;
				}
			}
		}
	}

	auto attackComponent = GetThisEnemy()->GetComponentByClass<UAttackComponent>();

	if (!attackComponent)
		return UDebugMessages::LogError(this, "could not find attack component, will not attack");

	if (bestSkill && bestEquipment) {
		if (bestEquipmentAttack > bestSkillAttack) {
			//do equipment attack
			//TODO
			UDebugMessages::LogError(this, "NOT IMPLEMENTED YET: Equipment based ai attack");
		}
		else {
			skillComponent->SetCurrentActiveSkill(bestSkill);
		}
	}
	else if (bestSkill) {
		skillComponent->SetCurrentActiveSkill(bestSkill);
	}
	else if (bestEquipment) {
		//do equipment attack
		//TODO
		UDebugMessages::LogError(this, "NOT IMPLEMENTED YET: Equipment based ai attack");
	}
	else return UDebugMessages::LogError(this, "no skill or equipment found, will not attack");

	auto sourceLoc = UGridUtilities::GetNormalisedGridLocation(GetThisEnemy()->GetActorLocation());

	if (GetBehaviourTargets().IsEmpty())
		return UDebugMessages::LogError(this, "no valid behvaiour target set");

	auto targetCharacter = GetBehaviourTargets()[0];

	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([attackComponent, sourceLoc, targetCharacter]
		{
			attackComponent->TryAttackTarget(sourceLoc, targetCharacter, false);
		},
		TStatId(), nullptr, ENamedThreads::GameThread);
}