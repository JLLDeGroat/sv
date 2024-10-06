// Fill out your copyright notice in the Description page of Project Settings.


#include "IndicatorClickComponent.h"
#include "Components/BoxComponent.h"
#include "../../../Utilities/SvUtilities.h"

// Sets default values for this component's properties
UIndicatorClickComponent::UIndicatorClickComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ClickComp"));

	if (GetOwner() && GetOwner()->GetRootComponent())
		BoxComponent->SetupAttachment(GetOwner()->GetRootComponent());

	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionObjectType(USvUtilities::GetClickableEnvironmentChannel());
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToChannel(USvUtilities::GetClickableEnvironmentChannel(), ECR_Block);
	BoxComponent->SetVisibility(true);
	BoxComponent->bHiddenInGame = false;
	BoxComponent->SetBoxExtent(FVector(50, 50, 32));
}


// Called when the game starts
void UIndicatorClickComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UIndicatorClickComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

