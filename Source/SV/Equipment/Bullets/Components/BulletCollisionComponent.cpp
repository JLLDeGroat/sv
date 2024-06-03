// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletCollisionComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../Utilities/SvUtilities.h"
#include "../../../Characters/Components/HitCapsuleComponent.h"
#include "../../../Characters/Components/HitBoxComponent.h"
#include "../../../Characters/Components/DamageRecieveComponent.h"
#include "BulletDetailsComponent.h"

#include "../../../Environment/EnvironmentActor.h"


UBulletCollisionComponent::UBulletCollisionComponent(const FObjectInitializer& ObjectInitializer)
	: UCapsuleComponent(ObjectInitializer) {
	SetCanEverAffectNavigation(false);
	SetGenerateOverlapEvents(true);

	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionObjectType(USvUtilities::GetBulletCollisionObjectChannel());
	SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionObjectChannel(), ECR_Overlap);
	SetCollisionResponseToChannel(USvUtilities::GetEnvironmentChannel(), ECR_Overlap);
	SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	OnComponentBeginOverlap.AddDynamic(this, &UBulletCollisionComponent::Overlapped);
}

void UBulletCollisionComponent::Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	UDebugMessages::LogDisplay(this, "overlapped " + OtherActor->GetName() + " comp: " + OtherComp->GetName());

	if (OtherComp->Implements<UHitComponent>()) {
		TScriptInterface<IHitComponent> hitComp = OtherComp;
		auto bulletDetails = GetOwner()->GetComponentByClass<UBulletDetailsComponent>();
		auto damageRecieve = OtherActor->GetComponentByClass<UDamageRecieveComponent>();
		if (!bulletDetails || !damageRecieve) {
			UDebugMessages::LogError(this, "no bullet details or damage recieve, ERROR, will do no damage");
			GetOwner()->Destroy();
			return;
		}

		damageRecieve->DoDamage(hitComp->GetHitDamageMultiplier(), bulletDetails->GetBaseDamage(), 
			GetOwner()->GetActorLocation(), bulletDetails->GetBaseImpulse());
		
		//TODO:
		//Create Blood spatter
		GetOwner()->Destroy();
	}

	//TODO: change environment tests
	else if (auto environment = Cast<AEnvironmentActor>(OtherActor)) {
		//TODO: Create environment spatter
		GetOwner()->Destroy();
	}
}
