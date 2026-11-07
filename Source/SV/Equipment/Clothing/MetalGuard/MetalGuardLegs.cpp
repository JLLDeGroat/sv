// Fill out your copyright notice in the Description page of Project Settings.

#include "MetalGuardLegs.h"
#include "../../../Utilities/SvUtilities.h"
#include "../Components/ClothingAttachComponent.h"
#include "../Components/ClothingHitBoxComponent.h"
#include "../Components/ClothingStatsComponent.h"

AMetalGuardLegs::AMetalGuardLegs(const FObjectInitializer &ObjectInitializer) : AClothingLegs(ObjectInitializer)
{

    LeftUpperLegMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftUpperLegMeshComponent"));
    RightUpperLegMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightUpperLegMeshComponent"));
    LeftLowerLegMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftLowerLegMeshComponent"));
    RightLowerLegMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightLowerLegMeshComponent"));

    auto lowerLegMesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/Clothing/MetalClothing/AnkleGuard.AnkleGuard'");
    auto upperLegMesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/Clothing/MetalClothing/ThighGuard.ThighGuard'");

    LeftUpperLegMeshComponent->SetStaticMesh(upperLegMesh);
    RightUpperLegMeshComponent->SetStaticMesh(upperLegMesh);

    LeftLowerLegMeshComponent->SetStaticMesh(lowerLegMesh);
    RightLowerLegMeshComponent->SetStaticMesh(lowerLegMesh);

    SetupClothingMeshComponent(LeftUpperLegMeshComponent);
    SetupClothingMeshComponent(RightUpperLegMeshComponent);
    SetupClothingMeshComponent(LeftLowerLegMeshComponent);
    SetupClothingMeshComponent(RightLowerLegMeshComponent);

    //(X=22.469778,Y=0.397562,Z=-3.523481)
    //(Pitch=70.000000,Yaw=21.000000,Roll=19.000000)

    AttachComponent->AddToAttachments(RightUpperLegMeshComponent,
                                      EAttachType::AT_RightThighGuard,
                                      FVector(22.469778f, 0.397562f, -3.523481f),
                                      FRotator(70, 21, 19),
                                      FVector(0.09f));

    //(X=19.376339,Y=0.117148,Z=4.771606)
    //(Pitch=292.669536,Yaw=117.101551,Roll=-107.470130)

    AttachComponent->AddToAttachments(LeftUpperLegMeshComponent,
                                      EAttachType::AT_LeftThighGuard,
                                      FVector(19.376339f, 0.117148f, 4.771606f),
                                      FRotator(292.669536f, 117.101551f, -107.470130f),
                                      FVector(0.09f));

    //(X=23.545234,Y=-1.615540,Z=2.171265)
    //(Pitch=89.000000,Yaw=-175.000000,Roll=-167.000000)
    AttachComponent->AddToAttachments(LeftLowerLegMeshComponent,
                                      EAttachType::AT_LeftShinGuard,
                                      FVector(23.545234f, -1.615540f, 2.171265f),
                                      FRotator(89, -175, -167),
                                      FVector(0.09f));

    //(X=23.862373,Y=-2.198080,Z=-1.708331)
    //(Pitch=74.000000,Yaw=-4.000000,Roll=-4.000000)
    AttachComponent->AddToAttachments(RightLowerLegMeshComponent,
                                      EAttachType::AT_RightShinGuard,
                                      FVector(23.862373f, -2.198080f, -1.708331f),
                                      FRotator(74, -4, 04),
                                      FVector(0.09f));

    LeftUpperLegHitBoxComponent = CreateDefaultSubobject<UClothingHitBoxComponent>(TEXT("LeftUpperHitBoxComp"));
    LeftUpperLegHitBoxComponent->SetupAttachment(LeftUpperLegMeshComponent);

    RightUpperLegHitBoxComponent = CreateDefaultSubobject<UClothingHitBoxComponent>(TEXT("RightUpperHitBoxComp"));
    RightUpperLegHitBoxComponent->SetupAttachment(RightUpperLegMeshComponent);

    LeftLowerLegHitBoxComponent = CreateDefaultSubobject<UClothingHitBoxComponent>(TEXT("LeftLowerHitBoxComp"));
    LeftLowerLegHitBoxComponent->SetupAttachment(LeftLowerLegMeshComponent);

    RightLowerLegHitBoxComponent = CreateDefaultSubobject<UClothingHitBoxComponent>(TEXT("RightLowerHitBoxComp"));
    RightLowerLegHitBoxComponent->SetupAttachment(RightLowerLegMeshComponent);

    // default value is 1
    auto relativeScale = FVector(1.2f);

    LeftUpperLegHitBoxComponent->SetRelativeLocation(FVector(-14.634135f, -102.142536f, 15.237602f));
    LeftUpperLegHitBoxComponent->SetRelativeRotation(FRotator(17, 0, 0));
    LeftUpperLegHitBoxComponent->SetBoxExtent(FVector(89.708026f, 99.603364f, 198.991007f));
    LeftUpperLegHitBoxComponent->SetRelativeScale3D(relativeScale);

    RightUpperLegHitBoxComponent->SetRelativeLocation(FVector(-14.634135f, -102.142536f, 15.237602f));
    RightUpperLegHitBoxComponent->SetRelativeRotation(FRotator(17, 0, 0));
    RightUpperLegHitBoxComponent->SetBoxExtent(FVector(89.708026f, 99.603364f, 198.991007f));
    RightUpperLegHitBoxComponent->SetRelativeScale3D(relativeScale);

    LeftLowerLegHitBoxComponent->SetRelativeLocation(FVector(-11.813350f, -82.573834f, -2.574135f));
    LeftLowerLegHitBoxComponent->SetBoxExtent(FVector(73.666653f, 69.045972f, 260.536703f));
    LeftLowerLegHitBoxComponent->SetRelativeRotation(FRotator(0, 0, -5));
    LeftLowerLegHitBoxComponent->SetRelativeScale3D(relativeScale);

    RightLowerLegHitBoxComponent->SetRelativeLocation(FVector(-11.813350f, -82.573834f, -2.574135f));
    RightLowerLegHitBoxComponent->SetBoxExtent(FVector(73.666653f, 69.045972f, 260.536703f));
    RightLowerLegHitBoxComponent->SetRelativeRotation(FRotator(0, 0, -5));
    RightLowerLegHitBoxComponent->SetRelativeScale3D(relativeScale);

    // setting Thickness
    LeftUpperLegHitBoxComponent->SetThickness(1);
    RightUpperLegHitBoxComponent->SetThickness(1);
    LeftLowerLegHitBoxComponent->SetThickness(1);
    RightLowerLegHitBoxComponent->SetThickness(1);

    ClothingType = EClothingType::CT_Legs;
    ClothingGrade = EClothingGrade::CG_Iron;

    StatsComponent->SetDamageMitigation(.2f);
    StatsComponent->SetHealth(50);
    StatsComponent->SetMaxHealth(50);
}