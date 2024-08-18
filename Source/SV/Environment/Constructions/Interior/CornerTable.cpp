// Fill out your copyright notice in the Description page of Project Settings.


#include "CornerTable.h"
#include "../../../Utilities/SvUtilities.h"

ACornerTable::ACornerTable(const FObjectInitializer& ObjectInitializer)
	:AEnvironmentActor(ObjectInitializer) {

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	TableLegMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Leg1"));
	TableLegMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Leg2"));

	RootComponent = TableMesh;

	TableLegMesh1->SetupAttachment(RootComponent);
	TableLegMesh2->SetupAttachment(RootComponent);

	TableMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);
	TableLegMesh1->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);
	TableLegMesh2->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);
}

void ACornerTable::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	auto table = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/BuildingInterior/CornerTable_Table.CornerTable_Table'");
	auto leg = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/BuildingInterior/CornerTable_TableLeg.CornerTable_TableLeg'");

	if (table) {
		TableMesh->SetStaticMesh(table);
	}

	if (leg) {
		TableLegMesh1->SetStaticMesh(leg);
		TableLegMesh2->SetStaticMesh(leg);
	}

	TableLegMesh2->SetRelativeLocation(FVector(83, -283, 31));
	TableLegMesh1->SetRelativeLocation(FVector(165, -55, 39));
}