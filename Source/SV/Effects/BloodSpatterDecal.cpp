// Fill out your copyright notice in the Description page of Project Settings.

#include "BloodSpatterDecal.h"
#include "Components/DecalComponent.h"
#include "../Utilities/SvUtilities.h"
// Sets default values
ABloodSpatterDecal::ABloodSpatterDecal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	auto sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = sceneComponent;

	Spatter1Component = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp1"));
	Spatter2Component = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp2"));
	Spatter3Component = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp3"));
	Spatter4Component = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp4"));
	Spatter5Component = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp5"));

	Spatter1Component->SetRelativeScale3D(FVector(.01f, .11f, .11f));
	Spatter2Component->SetRelativeScale3D(FVector(.01f, .11f, .11f));
	Spatter3Component->SetRelativeScale3D(FVector(.01f, .11f, .11f));
	Spatter4Component->SetRelativeScale3D(FVector(.01f, .11f, .11f));
	Spatter5Component->SetRelativeScale3D(FVector(.01f, .11f, .11f));

	Spatter1Component->SetupAttachment(RootComponent);
	Spatter2Component->SetupAttachment(RootComponent);
	Spatter3Component->SetupAttachment(RootComponent);
	Spatter4Component->SetupAttachment(RootComponent);
	Spatter5Component->SetupAttachment(RootComponent);

	Spatter1Component->SetVisibility(false);
	Spatter2Component->SetVisibility(false);
	Spatter3Component->SetVisibility(false);
	Spatter4Component->SetVisibility(false);
	Spatter5Component->SetVisibility(false);

	BaseMaterial = USvUtilities::GetMaterial("/Script/Engine.Material'/Game/Decals/Spatters/Floor/FloorSpatter1_M.FloorSpatter1_M'");
}

// Called when the game starts or when spawned
void ABloodSpatterDecal::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABloodSpatterDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma optimize("", off)
void ABloodSpatterDecal::SetupForWall()
{
	auto randomAmount = FMath::RandRange(1, 5);
	TArray<UDecalComponent *> components;
	for (int i = 0; i < randomAmount; i++)
	{
		if (i + 1 == 1)
			SetupWallComponent(Spatter1Component);
		else if (i + 1 == 2)
			SetupWallComponent(Spatter2Component);
		else if (i + 1 == 3)
			SetupWallComponent(Spatter3Component);
		else if (i + 1 == 4)
			SetupWallComponent(Spatter4Component);
		else if (i + 1 == 5)
			SetupWallComponent(Spatter5Component);
	}
}
void ABloodSpatterDecal::SetupForFloor(FVector originLocation, FRotator rotation, int distance)
{
	auto current = GetActorRotation();
	// SetActorRotation(rotation);
	auto randomAmount = FMath::RandRange(1, 2);
	TArray<UDecalComponent *> components;
	for (int i = 0; i < randomAmount; i++)
	{
		if (i + 1 == 1)
			SetupFloorComponent(originLocation, Spatter1Component, rotation, distance);
		else if (i + 1 == 2)
			SetupFloorComponent(originLocation, Spatter2Component, rotation, distance);
		else if (i + 1 == 3)
			SetupFloorComponent(originLocation, Spatter3Component, rotation, distance);
		else if (i + 1 == 4)
			SetupFloorComponent(originLocation, Spatter4Component, rotation, distance);
		else if (i + 1 == 5)
			SetupFloorComponent(originLocation, Spatter5Component, rotation, distance);
	}

	auto actorLoc = GetActorLocation();
	auto yaw = actorLoc.X < originLocation.X && actorLoc.Y < originLocation.Y ? 0 : 180;

	// auto newRot = FRotator(90, 0, rotation.Yaw + yaw);
	auto newRot = FRotator(90, GetActorRotation().Yaw, 0);
	Spatter1Component->SetRelativeRotation(newRot);
	Spatter2Component->SetRelativeRotation(newRot);
	Spatter3Component->SetRelativeRotation(newRot);
	Spatter4Component->SetRelativeRotation(newRot);
	Spatter5Component->SetRelativeRotation(newRot);
}
UDecalComponent *ABloodSpatterDecal::SetupWallComponent(UDecalComponent *component)
{
	auto instance = UMaterialInstanceDynamic::Create(USvUtilities::GetRandomBloodSpatterForWall(), this);

	auto randomY = FMath::RandRange(-30, 30);
	auto randomZ = FMath::RandRange(-30, 30);

	component->SetRelativeLocation(FVector(0, randomY, randomZ));

	component->SetDecalMaterial(instance);
	component->SetVisibility(true);
	return component;
}

UDecalComponent *ABloodSpatterDecal::SetupFloorComponent(FVector originLocation, UDecalComponent *component, FRotator rotation, int distance)
{
	auto instance = GenerateMaterial(); // UMaterialInstanceDynamic::Create(USvUtilities::GetRandomBloodSpatterForFloor(), this);

	auto worldLocation = originLocation + (FMath::RandRange(1, distance) + 50) * rotation.Vector();

	auto randomX = FMath::RandRange(-30, 30);
	auto randomY = FMath::RandRange(-30, 30);

	auto rightAngleVector = GetActorRightVector();
	auto additionalRandom = rightAngleVector * FMath::RandRange(-30, 30);
	additionalRandom.Z = 0;

	auto finalLocation = FVector(worldLocation.X, worldLocation.Y, GetActorLocation().Z) + additionalRandom;

	// component->SetRelativeLocation(relativeLoc);
	component->SetWorldLocation(finalLocation);

	// auto forwardVector = rotation.Vector();

	// auto additionalDistance = forwardVector * (FMath::RandRange(1, distance) * 50);
	// auto endLocation = originLocation + additionalDistance;

	// auto randomY = FMath::RandRange(0, 30);
	// auto randomZ = FMath::RandRange(-30, 30);

	// auto actorLoc = GetActorLocation();
	// if (originLocation.X < actorLoc.X)
	// 	randomZ = randomZ * -1;

	// if (originLocation.Y > actorLoc.Y)
	// 	randomY = randomY * -1;

	// component->SetRelativeLocation(FVector(0, additionalDistance.Y, additionalDistance.Z) + FVector(0, randomY, randomZ));

	component->SetDecalMaterial(instance);
	component->SetVisibility(true);
	return component;
}

UMaterialInstanceDynamic *ABloodSpatterDecal::GenerateMaterial()
{
	// /Script/Engine.Material'/Game/Decals/Spatters/Floor/FloorSpatter1_M.FloorSpatter1_M'
	if (!BaseMaterial)
	{
		return nullptr;
	}

	// Create a dynamic instance of your material
	UMaterialInstanceDynamic *DynamicMaterial =
		UMaterialInstanceDynamic::Create(BaseMaterial, this);

	if (!DynamicMaterial)
	{
		return nullptr;
	}

	// Set the scalar parameter
	DynamicMaterial->SetScalarParameterValue(
		FName("Intensity"),
		FMath::RandRange(1, 10));

	// Randomly select a texture numbered 1 through 15
	const int32 TextureIndex = FMath::RandRange(1, 15);

	// Build the asset path
	const FString TexturePath = FString::Printf(
		TEXT("/Game/Textures/Effects/FloorSplatters/Spatter_%d.Spatter_%d"),
		TextureIndex,
		TextureIndex);

	// Load the selected texture
	UTexture2D *SelectedTexture =
		LoadObject<UTexture2D>(nullptr, *TexturePath);

	if (SelectedTexture)
	{
		DynamicMaterial->SetTextureParameterValue(
			FName("Texture"),
			SelectedTexture);
	}

	return DynamicMaterial;
}
#pragma optimize("", on)