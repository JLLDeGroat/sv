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
void ABloodSpatterDecal::SetupForWall() {
	auto randomAmount = FMath::RandRange(1, 5);
	TArray<UDecalComponent*> components;
	for (int i = 0; i < randomAmount; i++) {
		if (i + 1 == 1) SetupWallComponent(Spatter1Component);
		else if (i + 1 == 2) SetupWallComponent(Spatter2Component);
		else if (i + 1 == 3) SetupWallComponent(Spatter3Component);
		else if (i + 1 == 4) SetupWallComponent(Spatter4Component);
		else if (i + 1 == 5) SetupWallComponent(Spatter5Component);
	}
}
void ABloodSpatterDecal::SetupForFloor(FRotator rotation) {
	auto current = GetActorRotation();
	//SetActorRotation(rotation);
	auto randomAmount = FMath::RandRange(1, 2);
	TArray<UDecalComponent*> components;
	for (int i = 0; i < randomAmount; i++) {
		if (i + 1 == 1) SetupFloorComponent(Spatter1Component);
		else if (i + 1 == 2) SetupFloorComponent(Spatter2Component);
		else if (i + 1 == 3) SetupFloorComponent(Spatter3Component);
		else if (i + 1 == 4) SetupFloorComponent(Spatter4Component);
		else if (i + 1 == 5) SetupFloorComponent(Spatter5Component);
	}

	auto newRot = FRotator(0, 0, rotation.Yaw + 180);
	Spatter1Component->SetRelativeRotation(newRot);
	Spatter2Component->SetRelativeRotation(newRot);
	Spatter3Component->SetRelativeRotation(newRot);
	Spatter4Component->SetRelativeRotation(newRot);
	Spatter5Component->SetRelativeRotation(newRot);
}
UDecalComponent* ABloodSpatterDecal::SetupWallComponent(UDecalComponent* component) {
	auto instance = UMaterialInstanceDynamic::Create(USvUtilities::GetRandomBloodSpatterForWall(), this);

	auto randomY = FMath::RandRange(-30, 30);
	auto randomZ = FMath::RandRange(-30, 30);

	component->SetRelativeLocation(FVector(0, randomY, randomZ));

	component->SetDecalMaterial(instance);
	component->SetVisibility(true);
	return component;
}
UDecalComponent* ABloodSpatterDecal::SetupFloorComponent(UDecalComponent* component) {
	auto instance = UMaterialInstanceDynamic::Create(USvUtilities::GetRandomBloodSpatterForFloor(), this);

	auto random = FMath::RandRange(-80, 80);
	
	component->SetRelativeLocation(FVector(0, random, random));

	component->SetDecalMaterial(instance);
	component->SetVisibility(true);
	return component;
}
#pragma optimize("", on)