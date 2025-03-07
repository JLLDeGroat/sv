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
#include "../Components/AnimSpeedComponent.h"
#include "../Components/ThrowableComponent.h"
#include "../Components/ActivateTogglesComponent.h"
#include "../Components/VaultObstacleComponent.h"
#include "../Components/HealthAndStatusWidgetComponent.h"
#include "../Components/ActionsComponent.h"
#include "../Components/UI/CharacterTileUIComponent.h"
#include "../Components/DestroyComponent.h"
#include "../Components/PickupResourceComponent.h"
#include "../Components/InventoryComponent.h"
#include "../Components/HitBoxComponent.h"
#include "../Components/HitCapsuleComponent.h"
#include "../Components/DropResourceComponent.h"
#include "../Components/CharacterCaptureComponent.h"
#include "../Components/HealthKitsComponent.h"
#include "../Components/FogHandlerComponent.h"
#include "../Components/ClimbLadderComponent.h"
#include "../Components/SkipObstacleComponent.h"
#include "../DandD/DeviantDirectiveComponent.h"

// Sets default values
ASolder::ASolder(const FObjectInitializer &ObjectInitializer) : ABaseCharacter(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridMovementComponent = CreateDefaultSubobject<UGridMovementComponent>(TEXT("GridMovement"));

	DetailsComponent->SetCharacterControl(ECharacterControl::CC_Player);
	DetailsComponent->SetHealth(100);

	TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("Targeting"));
	/// Script/Engine.SkeletalMesh'/Game/Characters/Soldier/BaseSoldier.BaseSoldier'
	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh(TEXT("SkeletalMesh'/Game/Characters/Soldier/ClothedPlayer.ClothedPlayer'"));
	if (skeletalMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(skeletalMesh.Object);
	}*/
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh(TEXT("Script/Engine.SkeletalMesh'/Game/Characters/Soldier/BaseSoldier.BaseSoldier'"));
	if (skeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(skeletalMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UClass> AnimObj(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Soldier/Anims/SolderAnim_Bp.SolderAnim_Bp_C'"));
	if (AnimObj.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimObj.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0, -20, -64));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Equipment"));
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("Attack"));

	DamageRecieveComponent = CreateDefaultSubobject<UDamageRecieveComponent>(TEXT("DamageRecieve"));

	DetailsComponent->AddMaxMovementPoints(5);
	DetailsComponent->AddMovementPoints(5);

	DetailsComponent->AddMaxActionPoints(2);
	DetailsComponent->AddActionPoints(2);

	DetailsComponent->SetCanVault(true);
	DetailsComponent->SetCanSkip(true);

	AnimSpeedComponent = CreateDefaultSubobject<UAnimSpeedComponent>(TEXT("AnimSpeed"));
	ThrowableComponent = CreateDefaultSubobject<UThrowableComponent>(TEXT("Throwable"));

	ActivateToggleComponent = CreateDefaultSubobject<UActivateTogglesComponent>(TEXT("ToggleComp"));

	VaultingComponent = CreateDefaultSubobject<UVaultObstacleComponent>(TEXT("Vault"));

	HealthAndStatusComponent = CreateDefaultSubobject<UHealthAndStatusWidgetComponent>(TEXT("StatusWidget"));
	HealthAndStatusComponent->SetupAttachment(RootComponent);
	HealthAndStatusComponent->SetRelativeLocation(FVector(0, 0, 130));

	ActionsComponent = CreateDefaultSubobject<UActionsComponent>(TEXT("Actions"));

	CharacterTileUIComponent = CreateDefaultSubobject<UCharacterTileUIComponent>(TEXT("UITile"));

	DestroyComponent = CreateDefaultSubobject<UDestroyComponent>(TEXT("DestroyComponent"));

	PickupResourceComponent = CreateDefaultSubobject<UPickupResourceComponent>(TEXT("PickupResource"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	LeftArmHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("LeftUpperArmHit"));
	LeftArmHitComponent->SetupAttachment(GetMesh(), FName("LeftArmSocket"));
	LeftArmHitComponent->SetRelativeRotation(FRotator(90, 180, 180));
	LeftArmHitComponent->SetRelativeLocation(FVector(16, 2.5, 1));
	LeftArmHitComponent->SetCapsuleRadius(7, false);
	LeftArmHitComponent->SetCapsuleHalfHeight(22, false);

	LeftForeArmHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("LeftForeArmHit"));
	LeftForeArmHitComponent->SetupAttachment(GetMesh(), FName("LeftForeArmSocket"));
	LeftForeArmHitComponent->SetRelativeRotation(FRotator(90, 180, 180));
	LeftForeArmHitComponent->SetRelativeLocation(FVector(16, 1, 1.6f));
	LeftForeArmHitComponent->SetCapsuleRadius(5, false);
	LeftForeArmHitComponent->SetCapsuleHalfHeight(20, false);

	RightArmHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("RightArmHit"));
	RightArmHitComponent->SetupAttachment(GetMesh(), FName("RightArmSocket"));
	RightArmHitComponent->SetRelativeRotation(FRotator(90, 180, 180));
	RightArmHitComponent->SetRelativeLocation(FVector(16, 2.5, 1));
	RightArmHitComponent->SetCapsuleRadius(6, false);
	RightArmHitComponent->SetCapsuleHalfHeight(22, false);

	RightForeArmHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("RightForeArmHit"));
	RightForeArmHitComponent->SetupAttachment(GetMesh(), FName("RightForeArmSocket"));
	RightForeArmHitComponent->SetRelativeRotation(FRotator(90, 180, 180));
	RightForeArmHitComponent->SetRelativeLocation(FVector(16, 1, 1.6f));
	RightForeArmHitComponent->SetCapsuleRadius(5, false);
	RightForeArmHitComponent->SetCapsuleHalfHeight(20, false);

	UpperTorsoHitComponent = CreateDefaultSubobject<UHitBoxComponent>(TEXT("BodySocketTwoHit"));
	UpperTorsoHitComponent->SetupAttachment(GetMesh(), FName("BodySocketTwo"));
	UpperTorsoHitComponent->SetBoxExtent(FVector(13, 10, 16));
	UpperTorsoHitComponent->SetRelativeRotation(FRotator(0, 5, 0));
	UpperTorsoHitComponent->SetRelativeLocation(FVector(15, 7, -2));
	UpperTorsoHitComponent->SetIsPreferredHitLocation(true);

	LowerTorsoHitComponent = CreateDefaultSubobject<UHitBoxComponent>(TEXT("BodySocketOneHit"));
	LowerTorsoHitComponent->SetupAttachment(GetMesh(), FName("BodySocketOne"));
	LowerTorsoHitComponent->SetBoxExtent(FVector(21, 10, 18));
	LowerTorsoHitComponent->SetRelativeRotation(FRotator(0, -21, 0));
	LowerTorsoHitComponent->SetRelativeLocation(FVector(23, 8, 0));
	LowerTorsoHitComponent->SetIsPreferredHitLocation(true); 

	LeftUpperLegHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("LeftUpperLegHit"));
	LeftUpperLegHitComponent->SetupAttachment(GetMesh(), FName("LeftUpLegSocket"));
	LeftUpperLegHitComponent->SetRelativeRotation(FRotator(82, 175, 170));
	LeftUpperLegHitComponent->SetRelativeLocation(FVector(21, -9, 1.7f));
	LeftUpperLegHitComponent->SetCapsuleRadius(7, false);
	LeftUpperLegHitComponent->SetCapsuleHalfHeight(30, false);

	RightUpperLegHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("RightUpperLegHit"));
	RightUpperLegHitComponent->SetupAttachment(GetMesh(), FName("RightUpLegSocket"));
	RightUpperLegHitComponent->SetRelativeRotation(FRotator(83, -3, -10));
	RightUpperLegHitComponent->SetRelativeLocation(FVector(21, -12, -4));
	RightUpperLegHitComponent->SetCapsuleRadius(7, false);
	RightUpperLegHitComponent->SetCapsuleHalfHeight(30, false);

	LeftLegHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("LeftLegHit"));
	LeftLegHitComponent->SetupAttachment(GetMesh(), FName("LeftLegSocket"));
	LeftLegHitComponent->SetRelativeRotation(FRotator(85.838082, 166, 167.246881));
	LeftLegHitComponent->SetRelativeLocation(FVector(26.034162, -8.716644, 0.657409));
	LeftLegHitComponent->SetCapsuleRadius(7, false);
	LeftLegHitComponent->SetCapsuleHalfHeight(27, false);

	RightLegHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("RightLegHit"));
	RightLegHitComponent->SetupAttachment(GetMesh(), FName("RightLegSocket"));
	RightLegHitComponent->SetRelativeRotation(FRotator(89, 6, 5));
	RightLegHitComponent->SetRelativeLocation(FVector(26.7f, -7, -3));
	RightLegHitComponent->SetCapsuleRadius(7, false);
	RightLegHitComponent->SetCapsuleHalfHeight(27, false);

	HeadHitComponent = CreateDefaultSubobject<UHitCapsuleComponent>(TEXT("HeadHit"));
	HeadHitComponent->SetupAttachment(GetMesh(), FName("HeadSocket"));
	HeadHitComponent->SetRelativeLocation(FVector(-12.136649, 11, 1.6f));
	HeadHitComponent->SetRelativeRotation(FRotator(90.000000, -174.948847, -174.948847));
	HeadHitComponent->SetCapsuleRadius(10, false);
	HeadHitComponent->SetCapsuleHalfHeight(12, false);
	HeadHitComponent->SetHitDimageMultiplier(2.0f);

	DropResourceComponent = CreateDefaultSubobject<UDropResourceComponent>(TEXT("DropResComp"));

	CharacterCaptureComponent = CreateDefaultSubobject<UCharacterCaptureComponent>(TEXT("CaptureComponent"));
	CharacterCaptureComponent->SetupAttachment(GetMesh(), FName("HeadSocket"));
	CharacterCaptureComponent->SetRelativeRotation(FRotator(161, -90, 91));
	CharacterCaptureComponent->SetRelativeLocation(FVector(-11, -9, -8));

	HealthKitsComponent = CreateDefaultSubobject<UHealthKitsComponent>(TEXT("HealthKits"));

	FogHandler = CreateDefaultSubobject<UFogHandlerComponent>(TEXT("FogHandler"));
	FogHandler->SetupAttachment(RootComponent);
	FogHandler->SetSphereRadius(400);

	ClimbLadderComponent = CreateDefaultSubobject<UClimbLadderComponent>(TEXT("ClimbLadder"));

	DDComponent = CreateDefaultSubobject<UDeviantDirectiveComponent>(TEXT("DeviantDirectives"));

	SkipObstacleComponent = CreateDefaultSubobject<USkipObstacleComponent>(TEXT("SkipObstacleComponent"));
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

bool ASolder::TryGetAsMoveable(TScriptInterface<IMovable> &Moveable)
{
	Moveable = this;
	return true;
}

UGridMovementComponent *ASolder::GetGridMovementComponent()
{
	return GridMovementComponent;
}