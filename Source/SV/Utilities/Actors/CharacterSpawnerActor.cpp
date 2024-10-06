// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSpawnerActor.h"
#include "../../Characters/BaseCharacter.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Characters/Components/EquipmentComponent.h"
#include "../../Characters/Components/HealthAndStatusWidgetComponent.h"
#include "../../Characters/Components/ThrowableComponent.h"
#include "../../Characters/Components/CharacterCaptureComponent.h"
#include "../../Equipment/Components/EquipmentDetailsComponent.h"
#include "../../Characters/Components/DamageRecieveComponent.h"
#include "../../Characters/Components/HealthKitsComponent.h"
#include "../../Equipment/Equipment.h"
#include "../SvUtilities.h"
#include "Components/StaticMeshComponent.h"
#include "../../Instance/SvGameInstance.h"
#include "../../Instance/Managers/CurrentGameDataManager.h"
#include "../GridUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

// Sets default values
ACharacterSpawnerActor::ACharacterSpawnerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootMesh->SetCanEverAffectNavigation(false);
	RootComponent = RootMesh;

	auto mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'");
	if (mesh)
		RootMesh->SetStaticMesh(mesh);
}

// Called when the game starts or when spawned
void ACharacterSpawnerActor::BeginPlay()
{
	Super::BeginPlay();

	if (!CharacterClass)
		return UDebugMessages::LogError(this, "no character class set for utility actor " + GetName());
	else {
		FActorSpawnParameters params;
		auto actor = GetWorld()->SpawnActor<ABaseCharacter>(CharacterClass, GetActorLocation(), FRotator::ZeroRotator, params);
		if (!actor) {
			UDebugMessages::LogError(this, "failed to spawn actor " + GetName());
		}
		else {
			auto equipmentComponent = actor->GetComponentByClass<UEquipmentComponent>();
			if (GunType != EGun::INVALID && equipmentComponent) {
				equipmentComponent->EquipPrimary(GunType);
			}

			if (SecondaryGunType != EGun::INVALID && equipmentComponent) {
				equipmentComponent->EquipSecondary(SecondaryGunType);
			}

			auto characterDetails = actor->GetComponentByClass<UCharacterDetailsComponent>();
			if (!characterDetails)
				return UDebugMessages::LogError(this, "could not get character details");

			if (OverrideCharacterName.Len() > 0) {
				if (characterDetails) characterDetails->SetCharacterName(OverrideCharacterName);

				auto statusWidget = actor->GetComponentByClass<UHealthAndStatusWidgetComponent>();
				if (statusWidget) statusWidget->SetName(OverrideCharacterName);
			}

			if (characterDetails->GetCharacterControl() == ECharacterControl::CC_Player) {
				auto gameInstance = USvUtilities::GetGameInstance(GetWorld());

				if (gameInstance) {
					auto currentGameDataManager = gameInstance->GetCurrentGameDataManager();
					if (!currentGameDataManager)
						return UDebugMessages::LogError(this, "failed to get currentGame data manager");

					auto currentGameData = currentGameDataManager->GetCurrentGameData();
					if (currentGameData) {
						auto memberId = currentGameData->AddCrewMember(characterDetails->GetCharacterName(), "", "", characterDetails->GetHealth(), characterDetails->GetMaxHealth());
						characterDetails->SetCharacterId(memberId);

						auto crewAmount = currentGameData->GetCrew().Num();
						auto captureComponent = actor->GetComponentByClass<UCharacterCaptureComponent>();
						if (captureComponent)
							captureComponent->SetToRenderTargetNumber(crewAmount);

						if (GrenadeAmount > 0) {
							for (int i = 0; i < GrenadeAmount; i++) {
								auto toolId = currentGameData->AddToolToCrew(EToolType::TT_Throwable, (uint8)EThrowable::T_Grenade);
								currentGameData->AssignToolToCrew(toolId, memberId);
								auto throwableComponent = actor->GetComponentByClass<UThrowableComponent>();
								throwableComponent->AddThrowable(EThrowable::T_Grenade, 1, toolId);
							}
						}

						if (HealthKitAmount > 0) {
							for (int i = 0; i < HealthKitAmount; i++) {
								auto kitId = currentGameData->AddToolToCrew(EToolType::TT_HealthKit, (uint8)EHealthKits::HK_Basic);
								currentGameData->AssignToolToCrew(kitId, memberId);
								auto throwableComponent = actor->GetComponentByClass<UHealthKitsComponent>();
								throwableComponent->AddHealthKits(EHealthKits::HK_Basic, 1, kitId);
							}
						}
					}

					if (GunType != EGun::INVALID) {
						auto gunId = currentGameData->AddPrimaryToCrew(GunType);
						auto primary = equipmentComponent->GetPrimaryEquipment();
						auto primaryEquipment = primary->GetComponentByClass<UEquipmentDetailsComponent>();
						primaryEquipment->SetEquipmentId(gunId);
					}

					if (SecondaryGunType != EGun::INVALID) {
						auto gunId = currentGameData->AddSecondaryToCrew(SecondaryGunType);
						auto secondary = equipmentComponent->GetSecondaryEquipment();
						auto secondaryEquipment = secondary->GetComponentByClass<UEquipmentDetailsComponent>();
						secondaryEquipment->SetEquipmentId(gunId);
					}
				}
			}

			if (TakeImmediateDamage > 0) {
				auto damageRecieveComponent = actor->GetComponentByClass<UDamageRecieveComponent>();
				if (!damageRecieveComponent)
					UDebugMessages::LogError(this, "cannot take immediate damage, no UDamageRecieveComponent component");
				else
					damageRecieveComponent->DoDamage(1, TakeImmediateDamage);
			}
		}
		Destroy();
	}
}

// Called every frame
void ACharacterSpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

