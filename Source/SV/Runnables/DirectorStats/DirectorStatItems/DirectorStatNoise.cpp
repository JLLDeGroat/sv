// Fill out your copyright notice in the Description page of Project Settings.

#include "DirectorStatNoise.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../Data/Game/Stats/FCrewMemberStats.h"

void UDirectorStatNoise::ActivateStatUpdate()
{
    auto world = GetWorld();

    if (!world)
        return UDebugMessages::LogError(this, "failed to get world");

    auto crewStats = GetCrewStats();
    crewStats->AddNoise(StatValue);

    UDebugMessages::LogDisplay(this, FString::SanitizeFloat(crewStats->GetNoise()));
}