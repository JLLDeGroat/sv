// Fill out your copyright notice in the Description page of Project Settings.

#include "ClothingHelpers.h"
#include "Clothing.h"

#include "MetalGuard/MetalGuardArms.h"
#include "MetalGuard/MetalGuardLegs.h"
#include "MetalGuard/MetalGuardChest.h"
#include "MetalGuard/MetalGuardShoulders.h"
#include "MetalGuard/MetalGuardHead.h"

UClass *UClothingHelpers::DetermineClothingClass(EClothingType clothingType, EClothingGrade clothingGrade)
{
    switch (clothingGrade)
    {
    case EClothingGrade::CG_Iron:
    {
        {
            switch (clothingType)
            {
            case EClothingType::CT_Chest:
                return AMetalGuardChest::StaticClass();
            case EClothingType::CT_Legs:
                return AMetalGuardLegs::StaticClass();
            case EClothingType::CT_Arms:
                return AMetalGuardArms::StaticClass();
            case EClothingType::CT_Shoulders:
                return AMetalGuardShoulders::StaticClass();
            case EClothingType::CT_Helmet:
                return AMetalGuardHead::StaticClass();

            default:
                return nullptr;
            }
        }
    }
    default:
        return nullptr;
    }
}