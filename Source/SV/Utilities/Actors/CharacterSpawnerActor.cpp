// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSpawnerActor.h"
#include "../../Characters/BaseCharacter.h"
#include "../SvUtilities.h"
#include "Components/StaticMeshComponent.h"
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
		Destroy();
	}
}

// Called every frame
void ACharacterSpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

