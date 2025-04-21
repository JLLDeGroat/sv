
#include "ExterminateDirector.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

UExterminateDirector::UExterminateDirector(const FObjectInitializer &ObjectInitializer)
    : UBaseDirectorGameType(ObjectInitializer)
{
}

float UExterminateDirector::GeneratePreassure()
{
    return 0.0f;
}