// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThrownOwnerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UThrownOwnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UThrownOwnerComponent();

	void SetThrownOwner(AActor* actor);
	AActor* GetThrownOwner();

protected:
	virtual void BeginPlay() override;	

private:

	UPROPERTY() AActor* ThrownOwner;

};
