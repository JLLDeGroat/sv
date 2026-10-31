// Fill out your copyright notice in the Description page of Project Settings.

#include "MetalGuardHead.h"
#include "../../../Utilities/SvUtilities.h"
#include "../Components/ClothingStatsComponent.h"
#include "../Components/ClothingAttachComponent.h"
#include "../Components/ClothingHitBoxComponent.h"

AMetalGuardHead::AMetalGuardHead(const FObjectInitializer &ObjectInitializer) : AClothingHelmet(ObjectInitializer)
{
    HeadMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMeshComponent"));

    auto mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/Clothing/MetalClothing/BaseHelm.BaseHelm'");
    HeadMeshComponent->SetStaticMesh(mesh);

    AttachComponent->AddToAttachments(HeadMeshComponent,
                                      EAttachType::AT_Helmet,
                                      FVector(-9.260621f, 2.724378f, 0.176184f),
                                      FRotator(90, 180, 0),
                                      FVector(2.3));

    SetupClothingMeshComponent(HeadMeshComponent);

    HelmetHitBoxComponent = CreateDefaultSubobject<UClothingHitBoxComponent>(TEXT("HelmetHitBox"));
    HelmetHitBoxComponent->SetupAttachment(HeadMeshComponent);

    HelmetHitBoxComponent->SetRelativeLocation(FVector(0, 0, 4.548245f));
    HelmetHitBoxComponent->SetBoxExtent(FVector(32, 32, 38));
    HelmetHitBoxComponent->SetRelativeScale3D(FVector(0.11f));

    // setting Thickness
    HelmetHitBoxComponent->SetThickness(1);

    ClothingType = EClothingType::CT_Helmet;
    ClothingGrade = EClothingGrade::CG_Iron;

    StatsComponent->SetDamageMitigation(.2f);
    StatsComponent->SetHealth(50);
}