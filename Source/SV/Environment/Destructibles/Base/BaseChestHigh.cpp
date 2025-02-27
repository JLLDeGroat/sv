#include "BaseChestHigh.h"
#include "GeometryCollection/GeometryCollection.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "../../../Utilities/SvUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "PhysicsField/PhysicsFieldComponent.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemNodes.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "../Components/DestructibleDetails.h"
#include "../Components/DestructibleHitComponent.h"
#include "../../Components/SkippableComponent.h"

ABaseChestHigh::ABaseChestHigh(const FObjectInitializer &ObjectInitializer) : AEnvironmentActor(ObjectInitializer)
{
    DestructibleMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
    RootComponent = DestructibleMesh;

    StaticBoxFallOff = CreateDefaultSubobject<UBoxFalloff>(TEXT("StaticBox"));
    StaticBoxFallOff->Magnitude = 500;
    StaticBoxFallOff->MinRange = 0;
    StaticBoxFallOff->MaxRange = 1.0f;
    StaticBoxFallOff->Default = 0;
    StaticBoxFallOff->Falloff = EFieldFalloffType::Field_FallOff_None;

    DisableBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("DisbleBoxComponent"));
    DisableBoxComponent->SetupAttachment(DestructibleMesh);

    DestructibleDetails = CreateDefaultSubobject<UDestructibleDetails>(TEXT("Details"));
    DestructibleHitComponent = CreateDefaultSubobject<UDestructibleHitComponent>(TEXT("HitComponent"));

    bDisableAreaAtStart = true;
}

void ABaseChestHigh::OnConstruction(const FTransform &Transform)
{
    Super::OnConstruction(Transform);
}
void ABaseChestHigh::BeginPlay()
{
    Super::BeginPlay();

    if (bDisableAreaAtStart)
    {
        StaticBoxFallOff->Magnitude = 1;
        StaticBoxFallOff->Transform = DisableBoxComponent->GetComponentTransform();

        // DrawDebugBox(GetWorld(), DisableBoxComponent->GetComponentLocation(), DisableBoxComponent->GetUnscaledBoxExtent(), FColor::Red, false, 100, 0, 1);

        UniformInt = NewObject<UUniformInteger>(this)->SetUniformInteger((int32)EObjectStateTypeEnum::Chaos_Object_Static);
        CullingField = NewObject<UCullingField>(this)->SetCullingField(StaticBoxFallOff, UniformInt, EFieldCullingOperationType::Field_Culling_Outside);

        DestructibleMesh->ApplyPhysicsField(
            true,
            EGeometryCollectionPhysicsTypeEnum::Chaos_DynamicState,
            nullptr,
            CullingField);
    }
}

void ABaseChestHigh::ReEnableForceOnHit()
{
    UDebugMessages::LogDisplay(this, "Renabled");
    DestructibleMesh->ApplyPhysicsField(
        false,
        EGeometryCollectionPhysicsTypeEnum::Chaos_DynamicState,
        nullptr,
        CullingField);
}