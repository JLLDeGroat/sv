// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleConstruct.h"

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
#include "../Components/TargetingComponent.h"

ARifleConstruct::ARifleConstruct(const FObjectInitializer& ObjectInitializer) : ABaseConstruct(ObjectInitializer) {
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Construct/Construct_Merged.Construct_Merged'"));
	if (skeletalMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(skeletalMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UClass> AnimObj(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Construct/Anims/GruntRifleAnim_Bp.GruntRifleAnim_Bp_C'"));
	if (AnimObj.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(AnimObj.Object);
	}

	AiComponent->SetMovementRoute(EAIBehaviourMoveRoutes::BMR_Range);
	AiComponent->SetAttackRoute(EAIBehaviourAttackRoutes::BAR_Range);

	TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("Targeting"));
}

UGridMovementComponent* ARifleConstruct::GetGridMovementComponent() {
	return GridMovementComponent;
}
void ARifleConstruct::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	//EquipmentComponent->EquipPrimary(EGun::G_ConstructRifle);
}
void ARifleConstruct::BeginPlay() {
	Super::BeginPlay();
	EquipmentComponent->EquipPrimary(EGun::G_ConstructRifle);
}