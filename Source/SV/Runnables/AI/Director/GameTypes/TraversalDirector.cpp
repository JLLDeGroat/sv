
#include "TraversalDirector.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

UTraversalDirector::UTraversalDirector(const FObjectInitializer &ObjectInitializer)
    : UBaseDirectorGameType(ObjectInitializer)
{
}

float UTraversalDirector::GeneratePreassure()
{
    return 0.0f;
}