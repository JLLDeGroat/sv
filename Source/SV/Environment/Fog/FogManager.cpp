#include "FogManager.h"
#include "../../Utilities/SvUtilities.h"
#include "Components/FogSectionComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Delegates/CharacterDelegates.h"

AFogManager::AFogManager()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Rootcomp"));
	RootComponent = SceneRoot;

	MaterialAsset = USvUtilities::GetMaterial("/Script/Engine.Material'/Game/Materials/Fog/BasicFog_M.BasicFog_M'");
	MeshAsset = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'");
}

// Called when the game starts or when spawned
void AFogManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFogManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LocationsToAdd.Num() == 0) {
		UDebugMessages::LogDisplay(this, "Total Locations " + FString::SanitizeFloat(FogComponents.Num()));
		SetActorTickEnabled(false);

		auto characterDelegates = UCharacterDelegates::GetInstance();

		if (!characterDelegates)
			UDebugMessages::LogError(this, "not found character delegates");

		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([characterDelegates] {
			characterDelegates->_OnFogGenerationComplete.Broadcast();
			}, TStatId(), nullptr, ENamedThreads::GameThread);
		return;
	}


	auto iterations = 0;
	while (LocationsToAdd.Num() > 0 && iterations < 10) {

		auto loc = LocationsToAdd[0];
		LocationsToAdd.RemoveAt(0);
		UFogSectionComponent* newMeshComp = NewObject<UFogSectionComponent>(this);

		if (newMeshComp)
		{
			// Attach it to the RootComponent or another component
			newMeshComp->SetupAttachment(RootComponent);

			// Optionally set properties on the new component
			newMeshComp->SetRelativeLocation(loc);
			newMeshComp->SetRelativeScale3D(FVector(1.f, 1.f, 2.f));
			newMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			newMeshComp->SetCollisionObjectType(USvUtilities::GetFogCollisionObjectChannel());
			newMeshComp->SetCanEverAffectNavigation(false);

			// Optionally set a mesh for the component
			//auto meshAsset = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'");

			//if (meshAsset) {
			newMeshComp->SetStaticMesh(MeshAsset);

			//auto material = USvUtilities::GetMaterial("/Script/Engine.Material'/Game/Materials/Fog/BasicFog_M.BasicFog_M'");
			auto instance = UMaterialInstanceDynamic::Create(MaterialAsset, this);
			newMeshComp->SetMaterialInstance(instance);

			FogComponents.Emplace(FFogComponent(newMeshComp));
			//}
			// Register the component to make it active
			newMeshComp->RegisterComponent();
		}

		iterations += 1;
	}
}

void AFogManager::AddComponentAtLocation(FVector loc) {
	if (!FogLocations.Contains(loc))
	{
		FogLocations.Add(loc, 1);
		LocationsToAdd.Emplace(loc);
	}
	SetActorTickEnabled(true);
}
