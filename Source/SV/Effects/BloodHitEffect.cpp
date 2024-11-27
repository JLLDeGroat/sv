// Fill out your copyright notice in the Description page of Project Settings.


#include "BloodHitEffect.h"
#include "NiagaraComponent.h"
#include "GeometryCacheComponent.h"
#include "GeometryCache.h"
#include "../Utilities/SvUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Characters/Monsters/GruntConstruct.h"
#include "../Characters/Monsters/RifleConstruct.h"

#include "../Characters/Humans/Solder.h"

// Sets default values

ABloodHitEffect::ABloodHitEffect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BloodSpatter = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Spatter"));
	RootComponent = BloodSpatter;

	BackBloodSplatter = CreateDefaultSubobject<UGeometryCacheComponent>(TEXT("GeometryCollection"));
	BackBloodSplatter->SetupAttachment(RootComponent);
	BackBloodSplatter->SetRelativeRotation(FRotator(0, 180, 0));
	BackBloodSplatter->SetPlaybackSpeed(SpatterSpeed);
	BackBloodSplatter->SetLooping(false);
	//BackBloodSplatter->OnPlaybackComplete.AddDynamic(this, &ABloobHitEffect::OnDelay);
	//BackBloodSplatter->Set
}

// Called when the game starts or when spawned
void ABloodHitEffect::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(OnAutoKill, this, &ABloodHitEffect::OnDelay, 1.5f, false);
}

void ABloodHitEffect::OnBackSpatterComplete() {
	UDebugMessages::LogDisplay(this, "OnBackSpatterComplete");
	BackBloodSplatter->SetVisibility(false);
}

void ABloodHitEffect::OnDelay() {
	Destroy();
}

void ABloodHitEffect::MoveBackSplatter(float movement) {
	UDebugMessages::LogError(this, "ABloodHitEffect::MoveBackSplatter This method does nothing");
	/*auto location = GetActorForwardVector() * movement;
	BackBloodSplatter->SetRelativeLocation(-location);*/
}

void ABloodHitEffect::SetBloodSpatterAssetFor(AActor* actor) {
	auto reference = "/Script/Niagara.NiagaraSystem'/Game/Effects/Spatter/HumanBloodSpatter_N.HumanBloodSpatter_N'";

	if (actor->IsA<ARifleConstruct>() || actor->IsA<AGruntConstruct>()) {
		reference = "/Script/Niagara.NiagaraSystem'/Game/Effects/Spatter/ConstructBloodSpatter_N.ConstructBloodSpatter_N'";
	}
	else if (actor->IsA<ASolder>()) {
		auto restCollection = USvUtilities::GetRandomBloodSpatterGeoCache();
		if (restCollection) {
			BackBloodSplatter->SetGeometryCache(restCollection);
			BackBloodSplatter->SetRelativeScale3D(FVector(10));
			BackBloodSplatter->SetRelativeLocation(FVector(-30, 0, 0));
			BackBloodSplatter->SetPlaybackSpeed(2.5f);
			BackBloodSplatter->SetStartTimeOffset(1);

			auto duration = BackBloodSplatter->GetDuration() / SpatterSpeed;
			UDebugMessages::LogDisplay(this, "found duration " + FString::SanitizeFloat(duration, 0));
			GetWorld()->GetTimerManager().SetTimer(OnBackSpatterCompleteAnim, this, &ABloodHitEffect::OnBackSpatterComplete, duration, false);
		}
	}
	else return UDebugMessages::LogError(this, "Failed to figure out what bloood to use");

	auto system = USvUtilities::GetNiagaraSystem(reference);
	if (system)
		BloodSpatter->SetAsset(system);
}
