// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionCompleteIndicator.h"
#include "../../Utilities/SvUtilities.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Characters/Components/ActionsComponent.h"
#include "Components/CapsuleComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/IndicatorActivatorComponent.h"
#include "Components/DecalComponent.h"


// Sets default values
AMissionCompleteIndicator::AMissionCompleteIndicator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActivatorIndicator->SetIndicatorType(EIndicatorType::IT_MissionExtract);

	BloodSpatterComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("BloodSpatterComponent"));

	//USvUtilities::GetRan
}

// Called when the game starts or when spawned
void AMissionCompleteIndicator::BeginPlay()
{
	Super::BeginPlay();
}
