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
#include "../Components/SuicideComponent.h"
#include "../Components/ModularSkeletonComponent.h"

ARifleConstruct::ARifleConstruct(const FObjectInitializer& ObjectInitializer) : ABaseConstruct(ObjectInitializer) {

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Construct/Construct_Merged.Construct_Merged'"));
	if (skeletalMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(skeletalMesh.Object);
	}

	HeadSkelComp = CreateDefaultSubobject<UModularSkeletonComponent>(TEXT("SkeleMesh2"));
	HeadSkelComp->SetupAttachment(GetMesh());

	LeftArmSkelComp = CreateDefaultSubobject<UModularSkeletonComponent>(TEXT("SkeleMesh3"));
	LeftArmSkelComp->SetupAttachment(GetMesh());

	LeftLegSkelComp = CreateDefaultSubobject<UModularSkeletonComponent>(TEXT("SkeleMesh4"));
	LeftLegSkelComp->SetupAttachment(GetMesh());

	RightArmSkelComp = CreateDefaultSubobject<UModularSkeletonComponent>(TEXT("SkeleMesh5"));
	RightArmSkelComp->SetupAttachment(GetMesh());

	RightLegSkelComp = CreateDefaultSubobject<UModularSkeletonComponent>(TEXT("SkeleMesh6"));
	RightLegSkelComp->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh1(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Construct/Construct_Merged_B.Construct_Merged_B'"));
	if (skeletalMesh1.Succeeded()) {
		GetMesh()->SetSkeletalMesh(skeletalMesh1.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh2(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Construct/Construct_Merged_H.Construct_Merged_H'"));
	if (skeletalMesh2.Succeeded()) {
		HeadSkelComp->SetSkeletalMesh(skeletalMesh2.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh3(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Construct/Construct_Merged_LA.Construct_Merged_LA'"));
	if (skeletalMesh3.Succeeded()) {
		LeftArmSkelComp->SetSkeletalMesh(skeletalMesh3.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh4(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Construct/Construct_Merged_LL.Construct_Merged_LL'"));
	if (skeletalMesh4.Succeeded()) {
		LeftLegSkelComp->SetSkeletalMesh(skeletalMesh4.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh5(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Construct/Construct_Merged_RA.Construct_Merged_RA'"));
	if (skeletalMesh5.Succeeded()) {
		RightArmSkelComp->SetSkeletalMesh(skeletalMesh5.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh6(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Construct/Construct_Merged_RL.Construct_Merged_RL'"));
	if (skeletalMesh6.Succeeded()) {
		RightLegSkelComp->SetSkeletalMesh(skeletalMesh6.Object);
	}

	static ConstructorHelpers::FObjectFinder<UClass> AnimObj(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Construct/Anims/GruntRifleAnim_Bp.GruntRifleAnim_Bp_C'"));
	if (AnimObj.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(AnimObj.Object);
	}

	AiComponent->SetMovementRoute(EAIBehaviourMoveRoutes::BMR_Range);
	AiComponent->SetAttackRoute(EAIBehaviourAttackRoutes::BAR_Range);

	TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("Targeting"));

	SuicideComponent = CreateDefaultSubobject<USuicideComponent>(TEXT("Suicide"));

	DetailsComponent->SetTargetIcon(ETargetIcon::TI_ConstructRange);
}

UGridMovementComponent* ARifleConstruct::GetGridMovementComponent() {
	return GridMovementComponent;
}
void ARifleConstruct::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
}

void ARifleConstruct::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (GetMesh()) {
		if (HeadSkelComp) {
			HeadSkelComp->SetLeaderPoseComponent(GetMesh());
			HeadSkelComp->SetupStaticMeshComp("/Script/GeometryCollectionEngine.GeometryCollection'/Game/Characters/Construct/Construct_GeoCollections/GC_ConstructHead.GC_ConstructHead'", FVector(-20, 10, 135));
		}
		if (LeftArmSkelComp) {
			LeftArmSkelComp->SetLeaderPoseComponent(GetMesh());
			LeftArmSkelComp->SetupStaticMeshComp("/Script/GeometryCollectionEngine.GeometryCollection'/Game/Characters/Construct/Construct_GeoCollections/ConstructLeftArm.ConstructLeftArm'", FVector(12.5f, -11.25f, 91), FRotator(-43, 90, -90));
		}
		if (RightArmSkelComp) {
			RightArmSkelComp->SetLeaderPoseComponent(GetMesh());
			RightArmSkelComp->SetupStaticMeshComp("/Script/GeometryCollectionEngine.GeometryCollection'/Game/Characters/Construct/Construct_GeoCollections/ConstructLeftArm.ConstructLeftArm'", FVector(-60, -11.25f, 91), FRotator(-43, 90, -90));
		}

		if (LeftLegSkelComp) LeftLegSkelComp->SetLeaderPoseComponent(GetMesh());
		if (RightLegSkelComp) RightLegSkelComp->SetLeaderPoseComponent(GetMesh());


		HeadHitComponent->SetModularComponent(HeadSkelComp);
		HeadSkelComp->SetModularPartHealth(10);
		HeadSkelComp->AddDebuffOnDestroy(EDebuffType::DBT_Accuracy, 75);

		LeftForeArmHitComponent->SetModularComponent(LeftArmSkelComp);
		LeftArmSkelComp->SetModularPartHealth(10);
		LeftArmSkelComp->AddDebuffOnDestroy(EDebuffType::DBT_Accuracy, 10);

		RightForeArmHitComponent->SetModularComponent(RightArmSkelComp);
		RightArmSkelComp->SetModularPartHealth(10);
		RightArmSkelComp->AddDebuffOnDestroy(EDebuffType::DBT_Accuracy, 10);
	}
}

void ARifleConstruct::BeginPlay() {
	Super::BeginPlay();
	EquipmentComponent->EquipPrimary(EGun::G_ConstructRifle);
	RightArmSkelComp->AddChildEquipment(EquipmentComponent->GetPrimaryEquipment());
}