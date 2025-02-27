#pragma once

#include "CoreMinimal.h"
#include "Async/TaskGraphInterfaces.h"
#include "Misc/ScopeLock.h"

class LoadBpActorTask : public FNonAbandonableTask
{
public:
    FString Reference;
    UObject **OutLoadedClass;

    // Constructor to initialize the task with the reference and output pointer
    LoadBpActorTask(FString InReference, UObject **InOutLoadedClass)
        : Reference(InReference), OutLoadedClass(InOutLoadedClass)
    {
    }

    void DoWork()
    {
        *OutLoadedClass = StaticLoadClass(UObject::StaticClass(), nullptr, *Reference);
    }

    // Optional: Providing a stat ID for this task
    FORCEINLINE TStatId GetStatId() const
    {
        RETURN_QUICK_DECLARE_CYCLE_STAT(LoadBpActorTask, STATGROUP_ThreadPoolAsyncTasks);
    }

    ENamedThreads::Type GetDesiredThread() { return ENamedThreads::GameThread; }
    static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }
    // This method is invoked to execute the task on the Game Thread
    void DoTask(ENamedThreads::Type CurrentThread, FGraphEventRef MyCompletionGraphEvent)
    {
        DoWork();
    }
};