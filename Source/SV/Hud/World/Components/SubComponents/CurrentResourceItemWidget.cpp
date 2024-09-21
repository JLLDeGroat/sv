// Fill out your copyright notice in the Description page of Project Settings.


#include "CurrentResourceItemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

void UCurrentResourceItemWidget::NativeConstruct() {
	Super::NativeConstruct();

}

void UCurrentResourceItemWidget::SetResourceImage(UTexture2D* texture) {
	if (!ResourceImage) return UDebugMessages::LogError(this, "failed to get resource image");

	ResourceImage->SetBrushFromTexture(texture);
}
void UCurrentResourceItemWidget::SetResourceText(FString text) {
	if (!ResourceText) return UDebugMessages::LogError(this, "failed to get resource text");

	ResourceText->SetText(FText::FromString(text));
}

EResourceType UCurrentResourceItemWidget::GetResourceType() {
	return ResourceType;
}
void UCurrentResourceItemWidget::SetResourceType(EResourceType rType) {
	ResourceType = rType;
}