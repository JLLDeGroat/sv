// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "../Utilities/GridUtilities.h"
#include "../Utilities/SvUtilities.h"
#include "AIController.h"
#include "GameFramework/Controller.h"
#include "Controllers/CharacterAIController.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/CharacterDetailsComponent.h"
#include "Components/TargetingComponent.h"
#include "../Delegates/CharacterDelegates.h"
#include "Components/HitBoxComponent.h"
#include "Components/HitCapsuleComponent.h"
#include "Components/EquipmentComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer) : ACharacter(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = ACharacterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(USvUtilities::GetClickableChannel(), ECR_Block);

	DetailsComponent = CreateDefaultSubobject<UCharacterDetailsComponent>(TEXT("DetailsComponent"));

	SvCharId = FGuid::NewGuid();
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	auto characterDelegates = UCharacterDelegates::GetInstance();
	if (characterDelegates)
		characterDelegates->_ReceiveNewCharacter.Broadcast(this);
	else UDebugMessages::LogError(this, "failed to get character delegates");
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ABaseCharacter::GetSelectableGridLocation() {
	return UGridUtilities::GetNormalisedGridLocation(GetActorLocation());
}

AAIController* ABaseCharacter::GetAIController() const {
	return Cast<AAIController>(GetController());
}

bool ABaseCharacter::TryGetAsMoveable(TScriptInterface<IMovable>& Moveable) {
	UDebugMessages::LogError(this, "tried to get base character as movable, ensure child class has overridden this if it should have movement");
	return false;
}

bool ABaseCharacter::IsControlType(ECharacterControl characterControl) {
	return DetailsComponent->GetCharacterControl() == characterControl;
}

void ABaseCharacter::TryVisualiseTargets() {
	auto targeting = GetComponentByClass<UTargetingComponent>();
	if (targeting) {
		targeting->DetermineTargetData();
	}
}
AActor* ABaseCharacter::GetAsActor() {
	return this;
}
FGuid ABaseCharacter::GetSvCharId() {
	return SvCharId;
}

TArray<TScriptInterface<IHitComponent>> ABaseCharacter::GetHitComponents() {
	TArray<UActorComponent*> components;
	GetComponents(components);

	TArray< TScriptInterface<IHitComponent>> response;

	for (UActorComponent* comp : components) {
		if (comp->IsA<UHitBoxComponent>() || 
			comp->IsA<UHitCapsuleComponent>()) 
		{
			response.Emplace(comp);
		}
	}

	return response;
}

void ABaseCharacter::UpdateActorVisibility(bool value) {
	SetActorHiddenInGame(!value);

	auto equipmentComponent = GetComponentByClass<UEquipmentComponent>();
	if (equipmentComponent) equipmentComponent->UpdateActorVisibility(value);
}