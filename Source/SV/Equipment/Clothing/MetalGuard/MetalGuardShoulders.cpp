// Fill out your copyright notice in the Description page of Project Settings.

#include "MetalGuardShoulders.h"
#include "../../../Utilities/SvUtilities.h"
#include "../Components/ClothingHitBoxComponent.h"
#include "../Components/ClothingAttachComponent.h"
#include "../Components/ClothingStatsComponent.h"

AMetalGuardShoulders::AMetalGuardShoulders(const FObjectInitializer &ObjectInitializer) : AClothingShoulders(ObjectInitializer)
{
    LeftShoulderMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftShoulderMeshComponent"));
    RightShoulderMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightShoulderMeshComponent"));

    auto mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/Clothing/MetalClothing/ShoulderGuard.ShoulderGuard'");

    LeftShoulderMeshComponent->SetStaticMesh(mesh);
    RightShoulderMeshComponent->SetStaticMesh(mesh);

    //(X=-4.164970,Y=-4.242190,Z=-1.018595)
    //(Pitch=344.910274,Yaw=180.640601,Roll=-256.000000)

    AttachComponent->AddToAttachments(LeftShoulderMeshComponent,
                                      EAttachType::AT_LeftShoulder,
                                      FVector(-6.373740f, -3.699934f, -3.563659f),
                                      FRotator(8, 164, 102),
                                      FVector(0.08f));

    AttachComponent->AddToAttachments(RightShoulderMeshComponent,
                                      EAttachType::AT_RightShoulder,
                                      FVector(-1.656582f, -3.176853f, 0.688059f),
                                      FRotator(8, 178, 85),
                                      FVector(0.08f));

    SetupClothingMeshComponent(LeftShoulderMeshComponent);
    SetupClothingMeshComponent(RightShoulderMeshComponent);

    RightShoulderHitBoxComponent = CreateDefaultSubobject<UClothingHitBoxComponent>(TEXT("RightShoulderHitBox"));
    RightShoulderHitBoxComponent->SetupAttachment(RightShoulderMeshComponent);

    RightShoulderHitBoxComponent->SetRelativeLocation(FVector(-34.556657f, -24.377789f, -65.516455f));
    RightShoulderHitBoxComponent->SetBoxExtent(FVector(75.835328f, 70.757779f, 92.071963f));
    RightShoulderHitBoxComponent->SetRelativeScale3D(FVector(1));

    LeftShoulderHitBoxComponent = CreateDefaultSubobject<UClothingHitBoxComponent>(TEXT("LeftShoulderHitBox"));
    LeftShoulderHitBoxComponent->SetupAttachment(LeftShoulderMeshComponent);

    //(X=-34.556657,Y=-24.377789,Z=-65.516455)
    //(X=75.835328,Y=70.757779,Z=92.071963)
    LeftShoulderHitBoxComponent->SetRelativeLocation(FVector(-34.556657f, -24.377789f, -65.516455f));
    LeftShoulderHitBoxComponent->SetBoxExtent(FVector(75.835328f, 70.757779f, 92.071963f));
    LeftShoulderHitBoxComponent->SetRelativeScale3D(FVector(1));

    // setting Thickness
    RightShoulderHitBoxComponent->SetThickness(1);
    LeftShoulderHitBoxComponent->SetThickness(1);

    ClothingType = EClothingType::CT_Shoulders;
    ClothingGrade = EClothingGrade::CG_Iron;

    StatsComponent->SetDamageMitigation(.2f);
    StatsComponent->SetHealth(50);
}