// Fill out your copyright notice in the Description page of Project Settings.


#include "AIComponent.h"
#include "../../Delegates/AIDelegates.h"
#include "Components/SphereComponent.h"
#include "CharacterDetailsComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Anim/CharAnimInstance.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Utilities/SvUtilities.h"

// Sets default values for this component's properties
UAIComponent::UAIComponent(const FObjectInitializer& ObjectInitializer)
	: UAnimAccessComponent(ObjectInitializer)
{
	bIsActiveAI = false;
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	AiActivationSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AiActivation"));
	AiActivationSphere->OnComponentBeginOverlap.AddDynamic(this, &UAIComponent::Overlapped);
	AiActivationSphere->SetGenerateOverlapEvents(true);
	AiActivationSphere->SetCollisionResponseToChannel(USvUtilities::GetFogCollisionObjectChannel(), ECR_Ignore);

	if (GetOwner() && GetOwner()->GetRootComponent())
		AiActivationSphere->SetupAttachment(GetOwner()->GetRootComponent());
}

void UAIComponent::SetActivationRadius(float radius) {
	Radius = radius;
}

void UAIComponent::BeginPlay() {
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(DelayFinish, this, &UAIComponent::OnDelayFinished, 1.0f, false);
}

void UAIComponent::BroadcastAiCompletedTask() {
	auto aiDelegates = UAIDelegates::GetInstance();

	if (!aiDelegates)
		return UDebugMessages::LogError(this, "failed to get ai Delegates");

	aiDelegates->_AICharacterFinishedBehaviour.Broadcast();
}

void UAIComponent::SetIsAiActive(bool val) {
	bIsActiveAI = val;
	AnimInstance->SetIsAiActive(val);
}
bool UAIComponent::GetIsActiveAi() {
	return bIsActiveAI;
}

void UAIComponent::Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	auto characterDetails = OtherActor->GetComponentByClass<UCharacterDetailsComponent>();
	if (characterDetails && OtherComp->IsA<UCapsuleComponent>() &&
		!OtherActor->GetComponentByClass<UAIComponent>())
	{
		SetIsAiActive(true);
	}
}

void UAIComponent::OnDelayFinished() {
	AiActivationSphere->SetSphereRadius(Radius);
}

void UAIComponent::SetAttackRoute(EAIBehaviourAttackRoutes aRoute) {
	AIAttackRoute = aRoute;
}
void UAIComponent::SetMovementRoute(EAIBehaviourMoveRoutes mRoute) {
	AIMoveRoute = mRoute;
}

EAIBehaviourAttackRoutes UAIComponent::GetAttackRoute() {
	return AIAttackRoute;
}
EAIBehaviourMoveRoutes UAIComponent::GetMovementRoute() {
	return AIMoveRoute;
}