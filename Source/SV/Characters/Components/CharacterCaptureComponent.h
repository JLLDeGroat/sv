// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "CharacterCaptureComponent.generated.h"

class UTextureRenderTarget2D;

/**
 *
 */
UCLASS()
class SV_API UCharacterCaptureComponent : public USceneCaptureComponent2D
{
	GENERATED_BODY()

public:

	UCharacterCaptureComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	void SetToRenderTargetNumber(int renderTargetNumber);

	UTextureRenderTarget2D* GetTextureRenderTarget();
	UTexture2D* ConvertRenderTargetToTexture2D();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UTextureRenderTarget2D* TextureRenderTarget;

};
