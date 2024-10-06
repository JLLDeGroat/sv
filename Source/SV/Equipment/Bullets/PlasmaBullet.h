// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseBullet.h"
#include "PlasmaBullet.generated.h"

class UBulletAuraComponent;

UCLASS()
class SV_API APlasmaBullet : public ABaseBullet
{
	GENERATED_BODY()

public:

	APlasmaBullet(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UBulletAuraComponent* BulletAuraComponent;

};
