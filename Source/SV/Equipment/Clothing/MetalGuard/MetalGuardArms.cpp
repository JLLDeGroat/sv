// Fill out your copyright notice in the Description page of Project Settings.

#include "MetalGuardArms.h"
#include "../../../Utilities/SvUtilities.h"
#include "../components/ClothingHitBoxComponent.h"
#include "../Components/ClothingAttachComponent.h"
#include "../Components/ClothingStatsComponent.h"

AMetalGuardArms::AMetalGuardArms(const FObjectInitializer &ObjectInitializer) : AClothingArms(ObjectInitializer)
{

    LeftUpperArmMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftUpperArmMeshComponent"));
    RightUpperArmMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightUpperArmMeshComponent"));
    LeftLowerArmMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftLowerArmMeshComponent"));
    RightLowerArmMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightLowerArmMeshComponent"));

    LeftUpperArmHitBoxComponent = CreateDefaultSubobject<UClothingHitBoxComponent>(TEXT("LeftUpperArmHitBoxComponent"));
    LeftUpperArmHitBoxComponent->SetupAttachment(LeftUpperArmMeshComponent);
    RightUpperArmHitBoxComponent = CreateDefaultSubobject<UClothingHitBoxComponent>(TEXT("RightUpperArmHitBoxComponent"));
    RightUpperArmHitBoxComponent->SetupAttachment(RightUpperArmMeshComponent);
    LeftLowerArmHitBoxComponent = CreateDefaultSubobject<UClothingHitBoxComponent>(TEXT("LeftLowerArmHitBoxComponent"));
    LeftLowerArmHitBoxComponent->SetupAttachment(LeftLowerArmMeshComponent);
    RightLowerArmHitBoxComponent = CreateDefaultSubobject<UClothingHitBoxComponent>(TEXT("RightLowerArmHitBoxComponent"));
    RightLowerArmHitBoxComponent->SetupAttachment(RightLowerArmMeshComponent);

    auto mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/Clothing/MetalClothing/ArmGuard.ArmGuard'");

    LeftUpperArmMeshComponent->SetStaticMesh(mesh);
    RightUpperArmMeshComponent->SetStaticMesh(mesh);
    LeftLowerArmMeshComponent->SetStaticMesh(mesh);
    RightLowerArmMeshComponent->SetStaticMesh(mesh);

    SetupClothingMeshComponent(LeftUpperArmMeshComponent);
    SetupClothingMeshComponent(RightUpperArmMeshComponent);
    SetupClothingMeshComponent(LeftLowerArmMeshComponent);
    SetupClothingMeshComponent(RightLowerArmMeshComponent);

    AttachComponent->AddToAttachments(LeftUpperArmMeshComponent,
                                      EAttachType::AT_LeftArmGuard,
                                      FVector(13.126855f, 2.926010f, 0.439025f),
                                      FRotator(0),
                                      FVector(0.135000f, 0.092500f, 0.152500f));

    AttachComponent->AddToAttachments(RightUpperArmMeshComponent,
                                      EAttachType::AT_RightArmGuard,
                                      FVector(12.964784f, 2.652613f, -0.148741f),
                                      FRotator(0),
                                      FVector(0.135000f, 0.092500f, 0.152500f));

    AttachComponent->AddToAttachments(LeftLowerArmMeshComponent,
                                      EAttachType::AT_LeftWristGuard,
                                      FVector(18.421028f, 0.552990f, 1.026696f),
                                      FRotator(4, 0, 0),
                                      FVector(0.1f, 0.11f, 0.11f));

    AttachComponent->AddToAttachments(RightLowerArmMeshComponent,
                                      EAttachType::AT_RightWristGuard,
                                      FVector(18.677414f, 0.684461f, -1.285496f),
                                      FRotator(-1, 0, 0),
                                      FVector(0.100000f, 0.110000f, 0.110000f));

    LeftUpperArmHitBoxComponent->SetRelativeLocation(FVector(0));
    LeftUpperArmHitBoxComponent->SetBoxExtent(FVector(115.540701f, 55.325806f, 43.029955f));
    LeftUpperArmHitBoxComponent->SetRelativeRotation(FRotator(0));
    LeftUpperArmHitBoxComponent->SetRelativeScale3D(FVector(1));

    RightUpperArmHitBoxComponent->SetRelativeLocation(FVector(0));
    RightUpperArmHitBoxComponent->SetBoxExtent(FVector(115.540701f, 55.325806f, 43.029955f));
    RightUpperArmHitBoxComponent->SetRelativeRotation(FRotator(0));
    RightUpperArmHitBoxComponent->SetRelativeScale3D(FVector(1));

    LeftLowerArmHitBoxComponent->SetRelativeLocation(FVector(0));
    LeftLowerArmHitBoxComponent->SetBoxExtent(FVector(124.600399f, 45.696108f, 46.067622f));
    LeftLowerArmHitBoxComponent->SetRelativeRotation(FRotator(0));
    LeftLowerArmHitBoxComponent->SetRelativeScale3D(FVector(1));

    RightLowerArmHitBoxComponent->SetRelativeLocation(FVector(0));
    RightLowerArmHitBoxComponent->SetBoxExtent(FVector(124.600399f, 45.696108f, 46.067622f));
    RightLowerArmHitBoxComponent->SetRelativeRotation(FRotator(0));
    RightLowerArmHitBoxComponent->SetRelativeScale3D(FVector(1));

    // setting Thickness
    LeftUpperArmHitBoxComponent->SetThickness(1);
    RightUpperArmHitBoxComponent->SetThickness(1);
    LeftLowerArmHitBoxComponent->SetThickness(1);
    RightLowerArmHitBoxComponent->SetThickness(1);

    ClothingType = EClothingType::CT_Arms;
    ClothingGrade = EClothingGrade::CG_Iron;

    StatsComponent->SetDamageMitigation(.2f);
    StatsComponent->SetHealth(50);
}