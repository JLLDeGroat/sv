// Fill out your copyright notice in the Description page of Project Settings.

#include "ArmourHitEffect.h"
#include "NiagaraComponent.h"
#include "../Utilities/SvUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

// Sets default values

AArmourHitEffect::AArmourHitEffect()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    MetalNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
    RootComponent = MetalNiagaraComponent;
}

// Called when the game starts or when spawned
void AArmourHitEffect::BeginPlay()
{
    Super::BeginPlay();

    auto system = USvUtilities::GetNiagaraSystem("/Script/Niagara.NiagaraSystem'/Game/Effects/MetalGuardHit_N.MetalGuardHit_N'");
    if (system)
        MetalNiagaraComponent->SetAsset(system);

    GetWorld()->GetTimerManager().SetTimer(OnAutoKill, this, &AArmourHitEffect::OnAutoKillCallback, 4.9f, false);
}

void AArmourHitEffect::OnAutoKillCallback()
{
    Destroy();
}