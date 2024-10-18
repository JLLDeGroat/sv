// Fill out your copyright notice in the Description page of Project Settings.


#include "BloodHitEffect.h"
#include "NiagaraComponent.h"
#include "../Utilities/SvUtilities.h"

// Sets default values

ABloodHitEffect::ABloodHitEffect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BloodSpatter = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Spatter"));
	RootComponent = BloodSpatter;


	auto system = USvUtilities::GetNiagaraSystem("/Script/Niagara.NiagaraSystem'/Game/Effects/Spatter/HumanBloodSpatter_N.HumanBloodSpatter_N'");
	if (system)
		BloodSpatter->SetAsset(system);
}

// Called when the game starts or when spawned
void ABloodHitEffect::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(OnAutoKill, this, &ABloodHitEffect::OnDelay, 1.5f, false);
}

void ABloodHitEffect::OnDelay() {
	Destroy();
}

