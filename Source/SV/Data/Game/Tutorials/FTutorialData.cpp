// Fill out your copyright notice in the Description page of Project Settings.


#include "FTutorialData.h"

FTutorialData::FTutorialData()
{

}

bool FTutorialData::GetCompletedTutorial(ETutorials tutorial) {
	if (TutorialsComplete.Find(tutorial))
		return TutorialsComplete[tutorial];

	return false;
}
void FTutorialData::SetCompletedTurorial(ETutorials tutorial) {
	if (TutorialsComplete.Find(tutorial))
		TutorialsComplete[tutorial] = true;
	else
		TutorialsComplete.Add(tutorial, true);
}