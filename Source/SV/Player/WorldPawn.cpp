// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/WorldPawnMovementComponent.h"
// Sets default values
AWorldPawn::AWorldPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComponent->InitCapsuleSize(35, 80);
	RootComponent = CapsuleComponent;

	SkeleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeleMesh"));
	SkeleMesh->SetupAttachment(RootComponent);
	SkeleMesh->SetRelativeLocation(FVector(0, -15, -58));
	SkeleMesh->SetRelativeRotation(FRotator(0, -90, 0));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Soldier/BaseSoldier.BaseSoldier'"));
	if (skeletalMesh.Succeeded()) SkeleMesh->SetSkeletalMesh(skeletalMesh.Object);

	static ConstructorHelpers::FObjectFinder<UClass> AnimObj(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Soldier/Anims/WorldSoldierAnim_Bp.WorldSoldierAnim_Bp_C'"));
	if (AnimObj.Succeeded()) SkeleMesh->SetAnimInstanceClass(AnimObj.Object);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeRotation(FRotator(-80, 0, 0));
	Camera->SetRelativeLocation(FVector(0, -25, 500));

	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spotlight"));
	SpotLightComponent->SetupAttachment(RootComponent);
	SpotLightComponent->SetRelativeRotation(FRotator(-90, 90, 0));
	SpotLightComponent->SetRelativeLocation(FVector(0, 0, 400));
	SpotLightComponent->SetOuterConeAngle(10);

	PawnMovementComponent = CreateDefaultSubobject<UWorldPawnMovementComponent>(TEXT("MovementComponent"));
}

// Called when the game starts or when spawned
void AWorldPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWorldPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AWorldPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

USkeletalMeshComponent* AWorldPawn::GetSkeleMeshComponent() {
	return SkeleMesh;
}