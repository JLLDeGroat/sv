// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletDetailsComponent.h"

// Sets default values for this component's properties
UBulletDetailsComponent::UBulletDetailsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBulletDetailsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBulletDetailsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UBulletDetailsComponent::GetBaseDamage() {
	return BaseDamage;
}
void UBulletDetailsComponent::SetBaseDamage(int damage) {
	BaseDamage = damage;
}
