// Fill out your copyright notice in the Description page of Project Settings.


#include "CrewDataManager.h"

void UCrewDataManager::SetCrewMembers(TArray<FCrewMember*> crewMembers) {
	for (int i = 0; i < crewMembers.Num(); i++)
		CrewMembers.Emplace(*crewMembers[i]);
}

void UCrewDataManager::AddCrewMember(FCrewMember* crewMember) {
	CrewMembers.Emplace(*crewMember);
}