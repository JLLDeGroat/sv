
#include "FMissionStats.h"


FMissionStats::FMissionStats() {
	KilledAmount = 0;
}

void FMissionStats::AddToKilledAmount() {
	KilledAmount += 1;
}

int FMissionStats::GetKilledAmount() {
	return KilledAmount;
}