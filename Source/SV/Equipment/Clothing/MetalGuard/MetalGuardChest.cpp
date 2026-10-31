// Fill out your copyright notice in the Description page of Project Settings.

#include "MetalGuardChest.h"
#include "../../../Utilities/SvUtilities.h"
#include "../components/ClothingHitBoxComponent.h"
#include "../Components/ClothingAttachComponent.h"
#include "../Components/ClothingStatsComponent.h"

AMetalGuardChest::AMetalGuardChest(const FObjectInitializer &ObjectInitializer) : AClothingChest(ObjectInitializer)
{
    // we need to add a list of box/sphere/capsule collision components to act as the area it will affect and absord damage
    // we need to add the mesh component as the visuals

    // we need to set the attackment point variable so other components know where to shove this component when equipping

    // clothing stats equipment for health/max and current

    auto chestPiece = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/Clothing/MetalClothing/MetalChest.MetalChest'");
    FrontChestMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontChestMeshComponent"));
    BackChestMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackChestMeshComponent"));

    BackChestMeshComponent->SetStaticMesh(chestPiece);
    FrontChestMeshComponent->SetStaticMesh(chestPiece);

    AttachComponent->AddToAttachments(BackChestMeshComponent,
                                      EAttachType::AT_BackChestGuard,
                                      FVector(7.517309, -0.427321, 3.565128f),
                                      FRotator(83, 177, 180),
                                      FVector(0.095f));

    AttachComponent->AddToAttachments(FrontChestMeshComponent,
                                      EAttachType::AT_FrontChestGuard,
                                      FVector(12.638880f, -1.272260f, 1.358612f),
                                      FRotator(97, -83, 94),
                                      FVector(0.08));

    SetupClothingMeshComponent(FrontChestMeshComponent);
    SetupClothingMeshComponent(BackChestMeshComponent);

    FrontChestHitBoxComponent = CreateDefaultSubobject<UClothingHitBoxComponent>(TEXT("FrontHitBox"));
    FrontChestHitBoxComponent->SetupAttachment(FrontChestMeshComponent);

    FrontChestHitBoxComponent->SetRelativeLocation(FVector(0, 0, 0));
    FrontChestHitBoxComponent->SetBoxExtent(FVector(125, 32, 200));
    FrontChestHitBoxComponent->SetRelativeScale3D(FVector(1));

    BackChestHitBoxComponent = CreateDefaultSubobject<UClothingHitBoxComponent>(TEXT("BackHitBox"));
    BackChestHitBoxComponent->SetupAttachment(BackChestMeshComponent);

    BackChestHitBoxComponent->SetRelativeLocation(FVector(0));
    BackChestHitBoxComponent->SetBoxExtent(FVector((125, 32, 200)));
    BackChestHitBoxComponent->SetRelativeScale3D(FVector(1));

    // setting Thickness
    FrontChestHitBoxComponent->SetThickness(1);
    BackChestHitBoxComponent->SetThickness(1);

    ClothingType = EClothingType::CT_Chest;
    ClothingGrade = EClothingGrade::CG_Iron;

    StatsComponent->SetDamageMitigation(.2f);
    StatsComponent->SetHealth(50);
}