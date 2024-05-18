// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Domain/SaveGameManager.h"
#include "JsonObjectConverter.h"
#include "FileManagementUtilities.generated.h"

/**
 * 
 */
UCLASS()
class VGFLMNGMNT_API UFileManagementUtilities : public UObject
{
	GENERATED_BODY()

public:

	template<typename OutStructType>
	static bool JsonToStruct(const FString& JsonString, OutStructType* OutStruct)
	{
		return FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, OutStruct);
	}

	template<typename InStructType>
	static FString StructToJson(const InStructType& InStruct)
	{
		auto jsonObject = FJsonObjectConverter::UStructToJsonObject(InStruct);
		FString OutputString;
		TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
		FJsonSerializer::Serialize(jsonObject.ToSharedRef(), Writer);

		return OutputString;
	}

	static TScriptInterface<ISaveManager> GetSaveManager() {
		static USaveGameManager* saveManager = 0; if (!saveManager)
		{
			saveManager = NewObject<USaveGameManager>();
			saveManager->AddToRoot();
		}

		return saveManager;
	}
};
