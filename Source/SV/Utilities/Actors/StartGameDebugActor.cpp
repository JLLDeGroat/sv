// Fill out your copyright notice in the Description page of Project Settings.

#include "StartGameDebugActor.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../SvUtilities.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(SVLogStartGameDebugActor)

// Sets default values
AStartGameDebugActor::AStartGameDebugActor()
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

void AStartGameDebugActor::BeginPlay()
{
	Super::BeginPlay();

	auto gameMode = USvUtilities::GetGameMode(GetWorld());

	if (bShouldStartAtGamePlayMode && !gameMode)
	{
		UDebugMessages::LogDisplay(this, "going to start new game mode");
		StartGamePlayGameMode();
	}
	else if (bShouldStartAtGamePlayMode)
	{
		UDebugMessages::LogDisplay(this, "already in gameplay mode leaving it here");
	}
	else
	{
		if (gameMode && !bShouldStartAtGamePlayMode)
		{
			UDebugMessages::LogDisplay(this, "Should start menu mode");
			StartMenuGameMode();
		}
	}
}
void AStartGameDebugActor::BeginDestroy()
{
	Super::BeginDestroy();
}

void AStartGameDebugActor::StartGamePlayGameMode()
{
	FName lName = FName("TopDownMap1");
	UGameplayStatics::OpenLevel(this, lName, true, "game=Class'/Script/SV.GameplayMode'?");
}

void AStartGameDebugActor::StartMenuGameMode()
{
	FName lName = FName("TopDownMap1");
	UGameplayStatics::OpenLevel(this, lName, true, "game=Class'/Script/SV.MenuGameMode'?");
}