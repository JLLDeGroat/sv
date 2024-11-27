// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularSkeletonComponent.h"
#include "HitBoxComponent.h"
#include "HitCapsuleComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "StatusEffectsComponent.h"
#include "GeometryCollection/GeometryCollection.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "PhysicsField/PhysicsFieldComponent.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemNodes.h"
#include "../../Equipment/Equipment.h"
#include "Field/FieldSystemObjects.h"
#include "Components/StaticMeshComponent.h"
#include "EquipmentComponent.h"
#include "AIComponent.h"

UModularSkeletonComponent::UModularSkeletonComponent(const FObjectInitializer& ObjectInitializer) :
	USkeletalMeshComponent(ObjectInitializer) {
	Health = -1;

	//StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));
	DestructibleMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Destruction"));
	DestructibleMesh->SetupAttachment(this);
	ExplosionField = CreateDefaultSubobject<UPhysicsFieldComponent>(TEXT("PhysicsField"));
	ExplosionField->SetupAttachment(DestructibleMesh);



	RadialVector = CreateDefaultSubobject<URadialVector>(TEXT("RadialVector"));
	RadialFallOff = CreateDefaultSubobject<URadialFalloff>(TEXT("RadialFalloff"));

	//DestructibleMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	DestructibleMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	DestructibleMesh->SetCollisionResponseToChannel(USvUtilities::GetEnvironmentChannel(), ECR_Block);
	DestructibleMesh->SetCollisionResponseToChannel(USvUtilities::GetFloorTargetChannel(), ECR_Block);
	/*DestructibleMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	DestructibleMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);*/
	//DestructibleMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	//static ConstructorHelpers::FObjectFinder<UGeometryCollection> GeometryCollectionAsset(TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/Characters/Construct/Construct_Head_Mesh_Construct_Head_Mesh/GC_ConstructHead.GC_ConstructHead'"));
	/*auto geoCollection = USvUtilities::GetGeometryCollection("/Script/GeometryCollectionEngine.GeometryCollection'/Game/Characters/Construct/Construct_Head_Mesh_Construct_Head_Mesh/GC_ConstructHead.GC_ConstructHead'");
	if (geoCollection) {
		DestructibleMesh->SetRestCollection(geoCollection);
		DestructibleMesh->SetSimulatePhysics(false);
		DestructibleMesh->SetEnableGravity(false);
		DestructibleMesh->SetupAttachment(GetAttachmentRoot());
	}*/
}

void UModularSkeletonComponent::SetModularPartHealth(int amount) {
	Health = amount;
}

void UModularSkeletonComponent::RemoveHealth(int amount) {
	Health -= amount;
	if (Health <= 0) {
		Health = 0;

		auto statusEffectComp = GetOwner()->GetComponentByClass<UStatusEffectsComponent>();
		if (!statusEffectComp)
			return UDebugMessages::LogError(this, "no status effect component found on owner");

		statusEffectComp->AddToDebuff(DebuffsOnDestroy);
	}
}

bool UModularSkeletonComponent::GetIsDead() {
	return Health <= 0;
}

void UModularSkeletonComponent::AddDebuffOnDestroy(EDebuffType debuff, float amount) {
	if (DebuffsOnDestroy.Find(debuff))
		DebuffsOnDestroy[debuff] += amount;
	else
		DebuffsOnDestroy.Add(debuff, amount);
}

TMap<EDebuffType, float> UModularSkeletonComponent::GetDebuffsOnDestruction() {
	return DebuffsOnDestroy;
}
#pragma optimize("", off)
void UModularSkeletonComponent::SetupStaticMeshComp(USkeletalMeshComponent* skeleMeshComp, FString meshref, FVector loc, FRotator rot) {
	auto geoCollection = USvUtilities::GetGeometryCollection(meshref);
	if (geoCollection) {
		if (GetOwner() && GetOwner()->GetRootComponent()) {
			//DestructibleMesh->AttachToComponent(skeleMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			//DestructibleMesh->SetCollisionResponseToChannel(USvUtilities::GetEnvironmentChannel(), ECR_Block);
			//DestructibleMesh->SetCollisionResponseToChannel(USvUtilities::GetFloorTargetChannel(), ECR_Block);
			//DestructibleMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
			//DestructibleMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
		}

		DestructibleMesh->SetRestCollection(geoCollection);
		DestructibleMesh->SetSimulatePhysics(false);
		DestructibleMesh->SetEnableGravity(false);
		DestructibleMesh->SetVisibility(false);
		DestructibleMesh->SetRelativeLocation(loc);
		DestructibleMesh->SetRelativeRotation(rot);
	}
}
#pragma optimize("", on)
void UModularSkeletonComponent::DestroyModularComponent(FVector direction) {
	SetVisibility(false);
	DestructibleMesh->SetVisibility(true);
	DestructibleMesh->SetSimulatePhysics(true);
	DestructibleMesh->SetEnableGravity(true);
	DestructibleMesh->SetCollisionResponseToChannel(USvUtilities::GetEnvironmentChannel(), ECR_Block);
	DestructibleMesh->SetCollisionResponseToChannel(USvUtilities::GetFloorTargetChannel(), ECR_Block);
	/*DestructibleMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	DestructibleMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);*/
	DestructibleMesh->RecreatePhysicsState();


	FVector CenterOfExplosion = DestructibleMesh->GetComponentLocation();
	float Magnitude = 50.0f;  // Strength of the explosion force
	float Radius = 5.0f;     // Radius within which the explosion affects

	URadialVector* RadialForceField = NewObject<URadialVector>(DestructibleMesh);
	RadialVector->SetRadialVector(Magnitude, CenterOfExplosion);


	RadialFallOff->Magnitude = 500000;
	RadialFallOff->MinRange = 1.0f;
	RadialFallOff->MaxRange = 50.0f;
	RadialFallOff->Default = 1.0f;
	RadialFallOff->Radius = 5;
	RadialFallOff->Position = DestructibleMesh->GetComponentLocation();
	RadialFallOff->Falloff = EFieldFalloffType::Field_Falloff_Linear;

	DestructibleMesh->ApplyPhysicsField(
		true,                                  // Enable field
		EGeometryCollectionPhysicsTypeEnum::Chaos_ExternalClusterStrain, // Applying a linear force
		nullptr,                               // MetaData (can be nullptr)
		RadialFallOff                    // The force field node
	);

	TObjectPtr<UFieldSystemMetaDataFilter> MetaData = NewObject<UFieldSystemMetaDataFilter>();
	MetaData->SetMetaDataFilterType(
		EFieldFilterType::Field_Filter_All,
		EFieldObjectType::Field_Object_All,
		EFieldPositionType::Field_Position_CenterOfMass);

	//DrawDebugSphere(GetWorld(), DestructibleMesh->GetComponentLocation(), RadialFallOff->Radius, 32, FColor::Red, false, 5.0f);

	// Apply the radial force to the geometry collection component (destructible mesh)
	RadialVector->SetRadialVector(75, CenterOfExplosion - (direction * 10));

	DestructibleMesh->ApplyPhysicsField(
		true,                                  // Enable field
		EGeometryCollectionPhysicsTypeEnum::Chaos_LinearImpulse, // Applying a linear force
		MetaData.Get(),                               // MetaData (can be nullptr)
		RadialVector                       // The force field node
	);

	DestructibleMesh->ApplyPhysicsField(
		true,                                  // Enable field
		EGeometryCollectionPhysicsTypeEnum::Chaos_AngularVelocity, // Applying a linear force
		MetaData.Get(),                               // MetaData (can be nullptr)
		RadialForceField                       // The force field node
	);

	if (!ChildEquipment.IsEmpty()) {
		for (int i = 0; i < ChildEquipment.Num(); i++) {

			auto owner = GetOwner();
			auto equipmentComponent = owner->GetComponentByClass<UEquipmentComponent>();

			auto aiComponent = owner->GetComponentByClass<UAIComponent>();

			if (ChildEquipment[i] == equipmentComponent->GetPrimaryEquipment() &&
				aiComponent && aiComponent->GetAttackRoute() == EAIBehaviourAttackRoutes::BAR_Range) {

				UDebugMessages::LogDisplay(this, "range actor lost its weapon, will now move to suicide");
				aiComponent->SetAttackRoute(EAIBehaviourAttackRoutes::BAR_Suicide);
				aiComponent->SetMovementRoute(EAIBehaviourMoveRoutes::BMR_Melee);
			}

			equipmentComponent->UnEquip(ChildEquipment[i]);
			ChildEquipment[i]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			auto components = ChildEquipment[i]->GetComponents();
			for (UActorComponent* comp : components) {
				if (auto staticComp = Cast<UStaticMeshComponent>(comp)) {
					staticComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
					staticComp->SetCollisionResponseToAllChannels(ECR_Ignore);
					staticComp->SetCollisionResponseToChannel(USvUtilities::GetEnvironmentChannel(), ECR_Block);
					staticComp->SetCollisionResponseToChannel(USvUtilities::GetFloorTargetChannel(), ECR_Block);
					staticComp->SetSimulatePhysics(true);
					UDebugMessages::LogError(this, "setting simulation");
				}
			}
		}
	}
}

void UModularSkeletonComponent::AddChildEquipment(AEquipment* equipment) {
	ChildEquipment.Emplace(equipment);
}