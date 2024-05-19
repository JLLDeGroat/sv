#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "BaseRunnable.generated.h"

/**
 *
 */
UCLASS()
class SV_API UBaseRunnable : public UObject, public FRunnable
{
	GENERATED_BODY()

public:

	virtual UBaseRunnable* Initialise(UWorld* world) {
		RandomStream = FRandomStream(FMath::RandRange(0, 999999));
		bIsAlive = true;
		World = world;
		return this;
	}

	UBaseRunnable* Begin() {
		Thread = FRunnableThread::Create(this, *FGuid::NewGuid().ToString(), 0, TPri_BelowNormal);
		return this;
	}

	virtual bool Init() {
		return true;
	}
	virtual uint32 Run() {
		ActivateThread();
		return 1;
	}
	virtual void Stop() {

	}

	void KillThread() {
		bIsAlive = false;
	}

	virtual UWorld* GetWorld() const override;

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UBaseThreads, STATGROUP_ThreadPoolAsyncTasks);
	}

	void EnsureCompletion() {
		KillThread();
		Thread->Kill(true);
	}

protected:

	virtual void ActivateThread() { };

	FRunnableThread* Thread;
	UPROPERTY() UWorld* World;
	UPROPERTY() bool bIsAlive;

	UPROPERTY() FRandomStream RandomStream;

	FORCEINLINE float GetRandomNumber(float min, float max) {
		return RandomStream.FRandRange(min, max);
	}

	FORCEINLINE void LogMessage(FString msg) {
		UDebugMessages::LogDisplay(this, msg);
	}
	FORCEINLINE void LogWarning(FString msg) {
		UDebugMessages::LogWarning(this, msg);
	}
	FORCEINLINE void LogError(FString msg) {
		UDebugMessages::LogError(this, msg);
	}

};
