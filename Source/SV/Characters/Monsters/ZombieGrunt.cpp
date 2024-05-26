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

	GetMesh()->SetRelativeLocation(FVector(0, -20, -69));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	DetailsComponent->SetCharacterControl(ECharacterControl::CC_AI);
	DetailsComponent->SetHealth(100);

	DamageRecieveComponent = CreateDefaultSubobject<UDamageRecieveComponent>(TEXT("DamageRecieve"));

	DetailsComponent->AddMaxActionPoints(2);
	DetailsComponent->AddActionPoints(2);

	DetailsComponent->AddMaxMovementPoints(6);
	DetailsComponent->AddMovementPoints(6);

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Equipment"));

	SkillComponent = CreateDefaultSubobject<USkillsComponent>(TEXT("Skills"));

	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("Attack"));


	LeftArmHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("LeftUpperArmHit"));
	LeftArmHitComponent->SetupAttachment(GetMesh(), FName("LeftArmSocket"));
	LeftArmHitComponent->SetRelativeRotation(FRotator(88, 0, 0));
	LeftArmHitComponent->SetRelativeLocation(FVector(15, 2, 0));
	LeftArmHitComponent->SetCapsuleHalfHeight(22, false);
	LeftArmHitComponent->SetCapsuleRadius(7, false);

	RightArmHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("RightArmHit"));
	RightArmHitComponent->SetupAttachment(GetMesh(), FName("RightArmSocket"));
	RightArmHitComponent->SetRelativeRotation(FRotator(88, 0, 0));
	RightArmHitComponent->SetRelativeLocation(FVector(15, 2, 0));
	RightArmHitComponent->SetCapsuleHalfHeight(22, false);
	RightArmHitComponent->SetCapsuleRadius(7, false);

	RightForeArmHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("RightForeArmHit"));
	RightForeArmHitComponent->SetupAttachment(GetMesh(), FName("RightForeArmSocket"));
	RightForeArmHitComponent->SetRelativeRotation(FRotator(88, 0, 0));
	RightForeArmHitComponent->SetRelativeLocation(FVector(18, 2, 0));
	RightForeArmHitComponent->SetCapsuleHalfHeight(15, false);
	RightForeArmHitComponent->SetCapsuleRadius(7, false);

	UpperTorsoHitComponent = CreateDefaultSubobject<UHitBoxComponent>(TEXT("BodySocketTwoHit"));
	UpperTorsoHitComponent->SetupAttachment(GetMesh(), FName("BodySocketTwo"));
	UpperTorsoHitComponent->SetBoxExtent(FVector(13, 12, 17));
	UpperTorsoHitComponent->SetRelativeRotation(FRotator(0, 12, 0));
	UpperTorsoHitComponent->SetRelativeLocation(FVector(17, 4, 0));

	LowerTorsoHitComponent = CreateDefaultSubobject<UHitBoxComponent>(TEXT("BodySocketOneHit"));
	LowerTorsoHitComponent->SetupAttachment(GetMesh(), FName("BodySocketOne"));
	LowerTorsoHitComponent->SetBoxExtent(FVector(23, 10, 12));
	LowerTorsoHitComponent->SetRelativeRotation(FRotator(0, -6, 0));
	LowerTorsoHitComponent->SetRelativeLocation(FVector(20, 2, 0));

	LeftUpperLegHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("LeftUpperLegHit"));
	LeftUpperLegHitComponent->SetupAttachment(GetMesh(), FName("LeftUpLegSocket"));
	LeftUpperLegHitComponent->SetRelativeRotation(FRotator(89, 0, 0));
	LeftUpperLegHitComponent->SetRelativeLocation(FVector(31, -8, 6));
	LeftUpperLegHitComponent->SetCapsuleHalfHeight(31, false);
	LeftUpperLegHitComponent->SetCapsuleRadius(9, false);

	RightUpperLegHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("RightUpperLegHit"));
	RightUpperLegHitComponent->SetupAttachment(GetMesh(), FName("RightUpLegSocket"));
	RightUpperLegHitComponent->SetRelativeRotation(FRotator(89, 0, 0));
	RightUpperLegHitComponent->SetRelativeLocation(FVector(31, -8, 6));
	RightUpperLegHitComponent->SetCapsuleHalfHeight(31, false);
	RightUpperLegHitComponent->SetCapsuleRadius(9, false);

	LeftLegHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("LeftLegHit"));
	LeftLegHitComponent->SetupAttachment(GetMesh(), FName("LeftLegSocket"));
	LeftLegHitComponent->SetRelativeRotation(FRotator(89, 6, 5));
	LeftLegHitComponent->SetRelativeLocation(FVector(26, -10, 2));
	LeftLegHitComponent->SetCapsuleHalfHeight(27, false);
	LeftLegHitComponent->SetCapsuleRadius(7, false);


	RightLegHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("RightLegHit"));
	RightLegHitComponent->SetupAttachment(GetMesh(), FName("RightLegSocket"));
	RightLegHitComponent->SetRelativeRotation(FRotator(89, 6, 5));
	RightLegHitComponent->SetRelativeLocation(FVector(26, -10, 2));
	RightLegHitComponent->SetCapsuleHalfHeight(27, false);
	RightLegHitComponent->SetCapsuleRadius(7, false);

	HeadHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("HeadHit"));
	HeadHitComponent->SetupAttachment(GetMesh(), FName("HeadSocket"));
	HeadHitComponent->SetRelativeLocation(FVector(-5, 7, 0));
	HeadHitComponent->SetRelativeRotation(FRotator(83, 0, 0));
	HeadHitComponent->SetCapsuleHalfHeight(15, false);
	HeadHitComponent->SetCapsuleRadius(12, false);
	HeadHitComponent->SetHitDimageMultiplier(2.0f);

	//LeftForeArmSocket
	//RightUpLegSocket
	//RightLegSocket

	//BodySocketTwo
	//BodySocketOne

}

UGridMovementComponent* AZombieGrunt::GetGridMovementComponent() {
	return GridMovementComponent;
}