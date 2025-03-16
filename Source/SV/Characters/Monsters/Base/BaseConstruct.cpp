// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseConstruct.h"
#include "../../../Utilities/SvUtilities.h"
#include "Components/StaticMeshComponent.h"
#include "../../Components/GridMovementComponent.h"
#include "../../Components/CharacterDetailsComponent.h"
#include "../../Components/HitBoxComponent.h"
#include "../../Components/HitCapsuleComponent.h"
#include "../../Components/DamageRecieveComponent.h"
#include "../../Components/EquipmentComponent.h"
#include "../../Components/CharacterDetailsComponent.h"
#include "../../Components/SkillsComponent.h"
#include "../../Components/AttackComponent.h"
#include "../../Components/AnimSpeedComponent.h"
#include "../../Components/HealthAndStatusWidgetComponent.h"
#include "../../Components/AIComponent.h"
#include "../../Components/SpawnInComponent.h"
#include "../../Components/DropResourceComponent.h"
#include "../../Components/FogHandlerComponent.h"
#include "../../Components/FogGenReceiveComponent.h"

ABaseConstruct::ABaseConstruct(const FObjectInitializer &ObjectInitializer)
	: ABaseCharacter(ObjectInitializer)
{

	GridMovementComponent = CreateDefaultSubobject<UGridMovementComponent>(TEXT("GridMovement"));

	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Construct/Construct_Merged.Construct_Merged'"));
	if (skeletalMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(skeletalMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UClass> AnimObj(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Construct/Anims/GruntMeleeAnim_Bp.GruntMeleeAnim_Bp_C'"));
	if (AnimObj.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(AnimObj.Object);
	}*/

	GetMesh()->SetRelativeLocation(FVector(0, -20, -69));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	DetailsComponent->SetCharacterControl(ECharacterControl::CC_AI);
	DetailsComponent->SetHealth(200);

	DamageRecieveComponent = CreateDefaultSubobject<UDamageRecieveComponent>(TEXT("DamageRecieve"));

	DetailsComponent->AddMaxActionPoints(2);
	DetailsComponent->AddActionPoints(2);

	DetailsComponent->AddMaxMovementPoints(6);
	DetailsComponent->AddMovementPoints(6);
	DetailsComponent->SetCharacterName("Construct Grunt");

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Equipment"));

	SkillComponent = CreateDefaultSubobject<USkillsComponent>(TEXT("Skills"));
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("Attack"));

	LeftArmHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("LeftUpperArmHit"));
	LeftArmHitComponent->SetupAttachment(GetMesh(), FName("LeftArmSocket"));
	LeftArmHitComponent->SetRelativeRotation(FRotator(88, 0, 0));
	LeftArmHitComponent->SetRelativeLocation(FVector(14, 0, 4));
	LeftArmHitComponent->SetCapsuleRadius(7, false);
	LeftArmHitComponent->SetCapsuleHalfHeight(22, false);

	LeftForeArmHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("LeftForeArmHit"));
	LeftForeArmHitComponent->SetupAttachment(GetMesh(), FName("LeftForeArmSocket"));
	LeftForeArmHitComponent->SetRelativeRotation(FRotator(90, 180, 175));
	LeftForeArmHitComponent->SetRelativeLocation(FVector(27, 2.6f, -3));
	LeftForeArmHitComponent->SetCapsuleRadius(7, false);
	LeftForeArmHitComponent->SetCapsuleHalfHeight(26, false);

	RightArmHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("RightArmHit"));
	RightArmHitComponent->SetupAttachment(GetMesh(), FName("RightArmSocket"));
	RightArmHitComponent->SetRelativeRotation(FRotator(88, 0, 0));
	RightArmHitComponent->SetRelativeLocation(FVector(14.7, 1.3, -4));
	RightArmHitComponent->SetCapsuleRadius(7, false);
	RightArmHitComponent->SetCapsuleHalfHeight(22, false);

	RightForeArmHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("RightForeArmHit"));
	RightForeArmHitComponent->SetupAttachment(GetMesh(), FName("RightForeArmSocket"));
	RightForeArmHitComponent->SetRelativeRotation(FRotator(90, 180, 175));
	RightForeArmHitComponent->SetRelativeLocation(FVector(27, 2.6f, -3));
	RightForeArmHitComponent->SetCapsuleRadius(7, false);
	RightForeArmHitComponent->SetCapsuleHalfHeight(26, false);

	UpperTorsoHitComponent = CreateDefaultSubobject<UHitBoxComponent>(TEXT("BodySocketTwoHit"));
	UpperTorsoHitComponent->SetupAttachment(GetMesh(), FName("BodySocketTwo"));
	UpperTorsoHitComponent->SetBoxExtent(FVector(13, 12, 17));
	UpperTorsoHitComponent->SetRelativeRotation(FRotator(0, 12, 0));
	UpperTorsoHitComponent->SetRelativeLocation(FVector(17, 4, 0));

	LowerTorsoHitComponent = CreateDefaultSubobject<UHitBoxComponent>(TEXT("BodySocketOneHit"));
	LowerTorsoHitComponent->SetupAttachment(GetMesh(), FName("BodySocketOne"));
	LowerTorsoHitComponent->SetBoxExtent(FVector(21, 10, 18));
	LowerTorsoHitComponent->SetRelativeRotation(FRotator(0, -6, 0));
	LowerTorsoHitComponent->SetRelativeLocation(FVector(23, 5, 1.6f));

	LeftUpperLegHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("LeftUpperLegHit"));
	LeftUpperLegHitComponent->SetupAttachment(GetMesh(), FName("LeftUpLegSocket"));
	LeftUpperLegHitComponent->SetRelativeRotation(FRotator(89, 0, 0));
	LeftUpperLegHitComponent->SetRelativeLocation(FVector(21.305139, 1.824009, 6.351508));
	LeftUpperLegHitComponent->SetCapsuleRadius(9, false);
	LeftUpperLegHitComponent->SetCapsuleHalfHeight(31, false);

	RightUpperLegHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("RightUpperLegHit"));
	RightUpperLegHitComponent->SetupAttachment(GetMesh(), FName("RightUpLegSocket"));
	RightUpperLegHitComponent->SetRelativeRotation(FRotator(89, 0, 0));
	RightUpperLegHitComponent->SetRelativeLocation(FVector(17.810672, 0, -7));
	RightUpperLegHitComponent->SetCapsuleRadius(9, false);
	RightUpperLegHitComponent->SetCapsuleHalfHeight(31, false);

	LeftLegHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("LeftLegHit"));
	LeftLegHitComponent->SetupAttachment(GetMesh(), FName("LeftLegSocket"));
	LeftLegHitComponent->SetRelativeRotation(FRotator(85.838082, -191.71701, -192.246881));
	LeftLegHitComponent->SetRelativeLocation(FVector(26.439232, 2.781889, 0));
	LeftLegHitComponent->SetCapsuleRadius(7, false);
	LeftLegHitComponent->SetCapsuleHalfHeight(27, false);

	RightLegHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("RightLegHit"));
	RightLegHitComponent->SetupAttachment(GetMesh(), FName("RightLegSocket"));
	RightLegHitComponent->SetRelativeRotation(FRotator(89, 6, 5));
	RightLegHitComponent->SetRelativeLocation(FVector(26, 3, -2));
	RightLegHitComponent->SetCapsuleRadius(7, false);
	RightLegHitComponent->SetCapsuleHalfHeight(27, false);

	HeadHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("HeadHit"));
	HeadHitComponent->SetupAttachment(GetMesh(), FName("HeadSocket"));
	HeadHitComponent->SetRelativeLocation(FVector(-12.136649, 0.978346, 0.554943));
	HeadHitComponent->SetRelativeRotation(FRotator(90.000000, -174.948847, -174.948847));
	HeadHitComponent->SetCapsuleRadius(12, false);
	HeadHitComponent->SetCapsuleHalfHeight(16.0f, false);
	HeadHitComponent->SetHitDimageMultiplier(2.0f);

	// LeftForeArmSocket
	// RightUpLegSocket
	// RightLegSocket

	// BodySocketTwo
	// BodySocketOne
	AnimSpeedComponent = CreateDefaultSubobject<UAnimSpeedComponent>(TEXT("AnimSpeed"));

	HealthAndStatusComponent = CreateDefaultSubobject<UHealthAndStatusWidgetComponent>(TEXT("StatusComponent"));
	HealthAndStatusComponent->SetupAttachment(RootComponent);
	HealthAndStatusComponent->SetRelativeLocation(FVector(0, 0, 130));

	AiComponent = CreateDefaultSubobject<UAIComponent>(TEXT("AiComponent"));
	AiComponent->SetActivationRadius(800);

	DropResourceComponent = CreateDefaultSubobject<UDropResourceComponent>(TEXT("DropRes"));

	SpawnInComponent = CreateDefaultSubobject<USpawnInComponent>(TEXT("SpawnIn"));

	DetailsComponent->SetBloodType(EBloodType::BT_PurpleCrystal);

	FogHandler = CreateDefaultSubobject<UFogHandlerComponent>(TEXT("FogHandler"));
	FogHandler->SetupAttachment(RootComponent);
	FogHandler->SetSphereRadius(150);
	FogHandler->SetIsAi(true);

	FogGenRecComponent = CreateDefaultSubobject<UFogGenReceiveComponent>(TEXT("FogGenRec"));
}

void ABaseConstruct::SetSkeletalMeshAndAnim(FString skele, FString anim)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh(*skele);
	if (skeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(skeletalMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UClass> AnimObj(*anim);
	if (AnimObj.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimObj.Object);
	}
}