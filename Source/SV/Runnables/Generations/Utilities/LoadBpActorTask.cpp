#include "LoadBpActorTask.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

void LoadBpActorTask::DoWork()
{
    // Make sure to load the class on the game thread
    *OutLoadedClass = StaticLoadClass(UObject::StaticClass(), nullptr, *Reference);

    if (!OutLoadedClass)
        UDebugMessages::LogDisplay(UObject::StaticClass(), "Could not find reference: " + Reference);
}