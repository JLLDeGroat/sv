// Fill out your copyright notice in the Description page of Project Settings.

#include "DebugMessages.h"

void UDebugMessages::DisplayMessage(FString msg)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, *msg);
}

void UDebugMessages::LogDisplay(UObject *contextObj, FString msg)
{
	UE_LOG(LogTemp, Display, TEXT("%s - %s"), *contextObj->GetName(), *msg);
}

void UDebugMessages::LogWarning(UObject *contextObj, FString msg)
{
	UE_LOG(LogTemp, Warning, TEXT("%s - %s"), *contextObj->GetName(), *msg);
}

void UDebugMessages::LogError(UObject *contextObj, FString msg)
{
	UE_LOG(LogTemp, Error, TEXT("%s - %s"), *contextObj->GetName(), *msg);
}

void UDebugMessages::LogDisplay(UObject *contextObj, FString msg, const FLogCategoryBase &category)
{
	FMsg::Logf(
		__FILE__,
		__LINE__,
		category.GetCategoryName(),
		ELogVerbosity::Display,
		TEXT("%s - %s"),
		*contextObj->GetName(),
		*msg);
}

void UDebugMessages::LogWarning(UObject *contextObj, FString msg, const FLogCategoryBase &category)
{
	FMsg::Logf(
		__FILE__,
		__LINE__,
		category.GetCategoryName(),
		ELogVerbosity::Warning,
		TEXT("%s - %s"),
		*contextObj->GetName(),
		*msg);
}

void UDebugMessages::LogError(UObject *contextObj, FString msg, const FLogCategoryBase &category)
{
	FMsg::Logf(
		__FILE__,
		__LINE__,
		category.GetCategoryName(),
		ELogVerbosity::Error,
		TEXT("%s - %s"),
		*contextObj->GetName(),
		*msg);
}
