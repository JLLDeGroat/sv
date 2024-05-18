// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentActor.h"
#include "../Utilities/SvUtilities.h"

// Sets default values
AEnvironmentActor::AEnvironmentActor(const FObjectInitializer& ObjectInitializer) : AActor(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AEnvironmentActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnvironmentActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnvironmentActor::SetupEnvironmentMeshComponent(UStaticMeshComponent* component) {
	component->SetCollisionResponseToChannel(USvUtilities::GetEnvironmentChannel(), ECR_Block);
	component->SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionChannel(), ECR_Overlap);
	component->SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionObjectChannel(), ECR_Overlap);
	component->SetGenerateOverlapEvents(true);
}
