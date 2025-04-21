// Fill out your copyright notice in the Description page of Project Settings.

#include "StatUpdateRunnable.h"
#include "StatItems/Base/BaseStatUpdate.h"
#include "StatItems/KilledEnemyStat.h"
#include "StatItems/DamagedEnemyStat.h"
#include "StatItems/MovementStat.h"

void UStatUpdateRunnable::ActivateThread()
{
	bCompleteStatItem = false;

	UBaseStatUpdate *baseStat = nullptr;

	switch (StatType)
	{
	case EStatisticType::ST_Kill:
		baseStat = NewObject<UKilledEnemyStat>(this);
		break;
	case EStatisticType::ST_DAMAGE:
		baseStat = NewObject<UDamagedEnemyStat>(this);
		break;
	case EStatisticType::ST_MOVEMENT:
		baseStat = NewObject<UMovementStat>(this);
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
