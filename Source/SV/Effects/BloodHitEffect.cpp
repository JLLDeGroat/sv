// Fill out your copyright notice in the Description page of Project Settings.


#include "BloodHitEffect.h"
#include "NiagaraComponent.h"
#include "GeometryCacheComponent.h"
#include "GeometryCache.h"
#include "../Utilities/SvUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

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

	auto system = USvUtilities::GetNiagaraSystem("/Script/Niagara.NiagaraSystem'/Game/Effects/Spatter/HumanBloodSpatter_N.HumanBloodSpatter_N'");
	if (system)
		BloodSpatter->SetAsset(system);
}

// Called when the game starts or when spawned
void ABloodHitEffect::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(OnAutoKill, this, &ABloodHitEffect::OnDelay, 1.5f, false);

	auto restCollection = USvUtilities::GetRandomBloodSpatterGeoCache();
	if (restCollection) {
		BackBloodSplatter->SetGeometryCache(restCollection);

		auto duration = BackBloodSplatter->GetDuration() / SpatterSpeed;
		UDebugMessages::LogDisplay(this, "found duration " + FString::SanitizeFloat(duration, 0));
		GetWorld()->GetTimerManager().SetTimer(OnBackSpatterCompleteAnim, this, &ABloodHitEffect::OnBackSpatterComplete, duration, false);
	}
}

void ABloodHitEffect::OnBackSpatterComplete() {
	UDebugMessages::LogDisplay(this, "OnBackSpatterComplete");
	BackBloodSplatter->SetVisibility(false);
}

void ABloodHitEffect::OnDelay() {
	Destroy();
}

void ABloodHitEffect::MoveBackSplatter(float movement) {
	/*auto location = GetActorForwardVector() * movement;
	BackBloodSplatter->SetRelativeLocation(-location);*/
}

