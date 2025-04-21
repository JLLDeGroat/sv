
#include "ExtractionDirector.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

UExtractionDirector::UExtractionDirector(const FObjectInitializer &ObjectInitializer)
    : UBaseDirectorGameType(ObjectInitializer)
{
}

float UExtractionDirector::GeneratePreassure()
{
    return 0.0f;
}