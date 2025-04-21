#pragma once

#include "CoreMinimal.h"
#include "Async/TaskGraphInterfaces.h"
#include "../../../Delegates/HudDelegates.h"
#include "Misc/ScopeLock.h"

class LoadLevelLoadingWidgetFogWaitTask : public FNonAbandonableTask
{
public:
    bool OutComplete;
    // Constructor to initialize the task with the reference and output pointer
    LoadLevelLoadingWidgetFogWaitTask()
        : OutComplete(false)
    {
    }

    void DoWork()
    {
        auto hudDelegates = UHudDelegates::GetInstance();
        if (hudDelegates)
        {
            hudDelegates->_LevelLoadingSetWaitForFog.Broadcast();
        }

        OutComplete = true;
    }

    bool GetIsComplete() const { return OutComplete; }

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