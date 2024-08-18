// Fill out your copyright notice in the Description page of Project Settings.


#include "NaturalRoad.h"
#include "../../Utilities/SvUtilities.h"
#include "Components/DecalComponent.h"

ANaturalRoad::ANaturalRoad(const FObjectInitializer& ObjectInitializer)
	: AEnvironmentActor(ObjectInitializer) {

	RoadDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	RootComponent = RoadDecalComponent;

	RoadDecalComponent->SetWorldRotation(FRotator(-90, 0, 0));
	RoadDecalComponent->SetWorldScale3D(FVector(.0055, .3f, .3f));
}


void ANaturalRoad::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);


	auto materialRef = "/Script/Engine.Material'/Game/Decals/RoadDecal1_M.RoadDecal1_M'";

	auto random = FMath::RandRange(1, 1001);

	if (random > 750) 
		materialRef = "/Script/Engine.Material'/Game/Decals/RoadDecal2_M.RoadDecal2_M'";
	else if (random > 500) 
		materialRef = "/Script/Engine.Material'/Game/Decals/RoadDecal3_M.RoadDecal3_M'";
	else if (random > 250) 
		materialRef = "/Script/Engine.Material'/Game/Decals/RoadDecal4_M.RoadDecal4_M'";

	auto randomRot = FMath::RandRange(-180, 180);

	RoadDecalComponent->SetWorldRotation(FRotator(-90, randomRot, 0));

	auto material = USvUtilities::GetMaterial(materialRef);
	if (material) {
		auto matInstance = UMaterialInstanceDynamic::Create(material, this);
		RoadDecalComponent->SetMaterial(0, matInstance);
	}
}