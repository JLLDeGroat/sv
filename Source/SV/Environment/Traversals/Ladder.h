// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnvironmentActor.h"
#include "Ladder.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UTraversalLocationsComponent;
class ANavLinkProxy;
class UNavLinkCustomComponent;
/**
 *
 */
UCLASS()
class SV_API ALadder : public AEnvironmentActor
{
	GENERATED_BODY()

public:

	ALadder(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* LadderMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UBoxComponent* LadderBottom;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UBoxComponent* LadderTop;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UBoxComponent* TraversalBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UTraversalLocationsComponent* TraversalLocationsComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) USceneComponent* RootSceneComp;

	UFUNCTION() void LadderTopOverlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void LadderBottomOverlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void LadderTopEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION() void LadderBottomEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//UPROPERTY(BlueprintReadWrite, EditAnywhere) ANavLinkProxy* NavProxy;
	///UPROPERTY(BlueprintReadWrite, EditAnywhere) UNavLinkCustomComponent* NavLinkComponent;

private:

	UPROPERTY() AActor* CharacterAtBottom;
	UPROPERTY() AActor* CharacterAtTop;

};
