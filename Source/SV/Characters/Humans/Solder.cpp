// Fill out your copyright notice in the Description page of Project Settings.


#include "Solder.h"
#include "../../Utilities/SvUtilities.h"
#include "Components/StaticMeshComponent.h"
#include "../Components/GridMovementComponent.h"
#include "../Components/CharacterDetailsComponent.h"
#include "../Components/TargetingComponent.h"
#include "../Components/EquipmentComponent.h"
#include "../Components/AttackComponent.h"
#include "../Components/DamageRecieveComponent.h"
// Sets default values
ASolder::ASolder(const FObjectInitializer& ObjectInitializer) : ABaseCharacter(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridMovementComponent = CreateDefaultSubobject<UGridMovementComponent>(TEXT("GridMovement"));

	//test stuff
	/*auto comp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TestMesh"));
	comp->SetupAttachment(RootComponent);
	auto mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/LevelPrototyping/Meshes/SM_Cylinder.SM_Cylinder'");
	comp->SetStaticMesh(mesh);
	comp->SetCollisionResponseToChannel(USvUtilities::GetClickableChannel(), ECR_Block);
	comp->SetCanEverAffectNavigation(false);*/

	DetailsComponent->SetCharacterControl(ECharacterControl::CC_Player);
	DetailsComponent->SetHealth(100);

	TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("Targeting"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh(TEXT("SkeletalMesh'/Game/Characters/Soldier/ClothedPlayer.ClothedPlayer'"));
	if (skeletalMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(skeletalMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UClass> AnimObj(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Soldier/Anims/SolderAnim_Bp.SolderAnim_Bp_C'"));
	if (AnimObj.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(AnimObj.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0, -20, -64));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Equipment"));
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("Attack"));
	DamageRecieveComponent = CreateDefaultSubobject<UDamageRecieveComponent>(TEXT("DamageRecieve"));
}

// Called when the game starts or when spawned
void ASolder::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

// Called every frame
void ASolder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASolder::TryGetAsMoveable(TScriptInterface<IMovable>& Moveable) {
	Moveable = this;
	return true;
}

UGridMovementComponent* ASolder::GetGridMovementComponent() {
	return GridMovementComponent;
}