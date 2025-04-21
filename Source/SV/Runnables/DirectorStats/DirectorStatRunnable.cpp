// Fill out your copyright notice in the Description page of Project Settings.

#include "DirectorStatRunnable.h"
#include "DirectorStatItems/Base/BaseDirectorStatUpdate.h"
#include "DirectorStatItems/DirectorStatNoise.h"
#include "DirectorStatItems/DirectorStatMovement.h"

void UDirectorStatRunnable::ActivateThread()
{
    bCompleteStatItem = false;

    UBaseDirectorStatUpdate *baseStat = nullptr;

    switch (StatType)
    {
    case EDirectorStatType::DS_NOISE:
        baseStat = NewObject<UDirectorStatNoise>(this);
        break;
    case EDirectorStatType::DS_MOVEMENT:
        baseStat = NewObject<UDirectorStatMovement>(this);
        break;

    default:
        UDebugMessages::LogError(this, "failed to get stat, stat not coded for TODO");
        break;
    }

    if (baseStat)
    {
        baseStat
            ->Setup(StatOwner, StatType, StatValue)
            ->ActivateStatUpdate();

        baseStat->ClearInternalFlags(EInternalObjectFlags::Async);
    }
    bCompleteStatItem = true;
}
