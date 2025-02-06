// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterCaptureComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "Engine/TextureRenderTarget2D.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "RenderUtils.h"

UCharacterCaptureComponent::UCharacterCaptureComponent(const FObjectInitializer &ObjectInitializer) : USceneCaptureComponent2D(ObjectInitializer)
{
	CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	bCaptureEveryFrame = false;
	bCaptureOnMovement = false;
}

void UCharacterCaptureComponent::BeginPlay()
{
	Super::BeginPlay();
}

UTextureRenderTarget2D *UCharacterCaptureComponent::GetTextureRenderTarget()
{
	return TextureRenderTarget;
}

void UCharacterCaptureComponent::SetToRenderTargetNumber(int renderTargetNumber)
{
	auto renderTarget = USvUtilities::GetRenderTarget2D(renderTargetNumber);
	if (renderTarget)
	{
		TextureRenderTarget = renderTarget;
		TextureTarget = TextureRenderTarget;
		UDebugMessages::LogDisplay(this, "Set Render target");
	}
	else
		UDebugMessages::LogError(this, "FAILED: to Set Render target");
}

UTexture2D *UCharacterCaptureComponent::ConvertRenderTargetToTexture2D()
{
	CaptureScene();

	if (!TextureRenderTarget)
		return nullptr;
	// Creates Texture2D to store TextureRenderTarget content
	UTexture2D *Texture = UTexture2D::CreateTransient(TextureRenderTarget->SizeX, TextureRenderTarget->SizeY, PF_B8G8R8A8);
#if WITH_EDITORONLY_DATA
	Texture->MipGenSettings = TMGS_NoMipmaps;
#endif
	Texture->SRGB = TextureRenderTarget->SRGB;

	// Read the pixels from the RenderTarget and store them in a FColor array
	TArray<FColor> SurfData;
	FRenderTarget *RenderTarget = TextureRenderTarget->GameThread_GetRenderTargetResource();
	RenderTarget->ReadPixels(SurfData);

	// Lock and copies the data between the textures
	void *TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	const int32 TextureDataSize = SurfData.Num() * 4;
	FMemory::Memcpy(TextureData, SurfData.GetData(), TextureDataSize);
	Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
	// Apply Texture changes to GPU memory
	Texture->UpdateResource();
	return Texture;
}