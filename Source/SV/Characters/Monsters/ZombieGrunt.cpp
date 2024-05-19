// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieGrunt.h"
#include "../../Utilities/SvUtilities.h"
#include "Components/StaticMeshComponent.h"
#include "../Components/GridMovementComponent.h"
#include "../Components/CharacterDetailsComponent.h"
#include "../Components/HitBoxComponent.h"
#include "../Components/HitCapsuleComponent.h"
#include "../Components/DamageRecieveComponent.h"
#include "../Components/CharacterDetailsComponent.h"

AZombieGrunt::AZombieGrunt(const FObjectInitializer& ObjectInitializer) : ABaseCharacter(ObjectInitializer) {

	GridMovementComponent = CreateDefaultSubobject<UGridMovementComponent>(TEXT("GridMovement"));

	//test stuff
	auto comp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TestMesh"));
	comp->SetupAttachment(RootComponent);
	auto mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/LevelPrototyping/Meshes/SM_Cylinder.SM_Cylinder'");
	comp->SetStaticMesh(mesh);
	comp->SetCollisionResponseToChannel(USvUtilities::GetClickableChannel(), ECR_Block);
	comp->SetCanEverAffectNavigation(false);

	DetailsComponent->SetCharacterControl(ECharacterControl::CC_AI);
	DetailsComponent->SetHealth(100);

	BodyHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("BodyComponent"));
	BodyHitComponent->SetupAttachment(RootComponent);

	DamageRecieveComponent = CreateDefaultSubobject<UDamageRecieveComponent>(TEXT("DamageRecieve"));

	CharacterDetailsComponent = CreateDefaultSubobject<UCharacterDetailsComponent>(TEXT("CharacterDetails"));

	CharacterDetailsComponent->AddMaxActionPoints(2);
	CharacterDetailsComponent->AddActionPoints(2);

	CharacterDetailsComponent->AddMaxMovementPoints(6);
	CharacterDetailsComponent->AddMovementPoints(6);
}

UGridMovementComponent* AZombieGrunt::GetGridMovementComponent() {
	return GridMovementComponent;
}