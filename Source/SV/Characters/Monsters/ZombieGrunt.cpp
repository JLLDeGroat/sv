// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieGrunt.h"
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

AZombieGrunt::AZombieGrunt(const FObjectInitializer& ObjectInitializer) : ABaseCharacter(ObjectInitializer) {

	GridMovementComponent = CreateDefaultSubobject<UGridMovementComponent>(TEXT("GridMovement"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Zombie/Spitter1.Spitter1'"));
	if (skeletalMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(skeletalMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UClass> AnimObj(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Zombie/Anim/ZombieGruntAnim_Bp.ZombieGruntAnim_Bp_C'"));
	if (AnimObj.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(AnimObj.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0, -20, -64));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	DetailsComponent->SetCharacterControl(ECharacterControl::CC_AI);
	DetailsComponent->SetHealth(100);

	BodyHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("BodyComponent"));
	BodyHitComponent->SetupAttachment(RootComponent);

	DamageRecieveComponent = CreateDefaultSubobject<UDamageRecieveComponent>(TEXT("DamageRecieve"));

	DetailsComponent->AddMaxActionPoints(2);
	DetailsComponent->AddActionPoints(2);

	DetailsComponent->AddMaxMovementPoints(6);
	DetailsComponent->AddMovementPoints(6);

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Equipment"));

	SkillComponent = CreateDefaultSubobject<USkillsComponent>(TEXT("Skills"));

	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("Attack"));
}

UGridMovementComponent* AZombieGrunt::GetGridMovementComponent() {
	return GridMovementComponent;
}