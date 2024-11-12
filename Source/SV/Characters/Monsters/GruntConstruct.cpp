// Fill out your copyright notice in the Description page of Project Settings.


#include "GruntConstruct.h"

#include "../../Utilities/SvUtilities.h"
#include "Components/StaticMeshComponent.h"
#include "../Components/GridMovementComponent.h"
#include "../Components/CharacterDetailsComponent.h"
#include "../Components/HitBoxComponent.h"
#include "../Components/HitCapsuleComponent.h"
#include "../Components/DamageRecieveComponent.h"
#include "../Components/EquipmentComponent.h"
#include "../Components/CharacterDetailsComponent.h"
#include "../Components/SkillsComponent.h"
#include "../Components/AttackComponent.h"
#include "../Components/AnimSpeedComponent.h"
#include "../Components/HealthAndStatusWidgetComponent.h"
#include "../Components/AIComponent.h"
#include "../Components/SpawnInComponent.h"
#include "../Components/DropResourceComponent.h"

AGruntConstruct::AGruntConstruct(const FObjectInitializer& ObjectInitializer) : ABaseConstruct(ObjectInitializer) {
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Construct/Construct_Merged.Construct_Merged'"));
	if (skeletalMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(skeletalMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UClass> AnimObj(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Construct/Anims/GruntMeleeAnim_Bp.GruntMeleeAnim_Bp_C'"));
	if (AnimObj.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(AnimObj.Object);
	}

	AiComponent->SetMovementRoute(EAIBehaviourMoveRoutes::BMR_Melee);
	AiComponent->SetAttackRoute(EAIBehaviourAttackRoutes::BAR_Melee);

	DetailsComponent->SetTargetIcon(ETargetIcon::TI_ConstructMelee);
}

UGridMovementComponent* AGruntConstruct::GetGridMovementComponent() {
	return GridMovementComponent;
}