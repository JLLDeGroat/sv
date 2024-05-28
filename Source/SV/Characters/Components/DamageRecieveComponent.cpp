// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageRecieveComponent.h"
#include "CharacterDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Delegates/CharacterDelegates.h"
#include "../../Interfaces/SvChar.h"
// Sets default values for this component's properties
UDamageRecieveComponent::UDamageRecieveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDamageRecieveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

// Called every frame
void UDamageRecieveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDamageRecieveComponent::DoDamage(float multiplier, int damage) {
	auto details = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
	if (!details) {
		UDebugMessages::LogError(this, "No details component, cannot take damage");
		return;
	}

	int total = multiplier * damage;

	UDebugMessages::LogDisplay(this, "took " + FString::SanitizeFloat(total, 0) + " damage.");
	bool isDead = false;
	details->RemoveHealth(total, isDead);

	if (isDead) {
		auto characterDelegates = UCharacterDelegates::GetInstance();

		if (!characterDelegates) 
			return UDebugMessages::LogError(this, "failed to get character Delegates, wont kill soldier");


		TScriptInterface<ISvChar> ownerAsCharacter = GetOwner();
		characterDelegates->_RemoveCharacter.Broadcast(ownerAsCharacter->GetSvCharId());
		GetOwner()->Destroy();
		//TODO: 
		// this should do something more better
	}
}