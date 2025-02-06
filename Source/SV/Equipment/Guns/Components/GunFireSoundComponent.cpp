// Fill out your copyright notice in the Description page of Project Settings.

#include "GunFireSoundComponent.h"
#include "../../../Utilities/SvUtilities.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

// Define the static arrays outside of any function
FString UGunFireSoundComponent::RifleGunFireSounds[9] = {
	"/Script/Engine.SoundWave'/Game/Equipment/Sounds/Rifle/Fire/Audio_1.Audio_1'",
	"/Script/Engine.SoundWave'/Game/Equipment/Sounds/Rifle/Fire/Audio_2.Audio_2'",
	"/Script/Engine.SoundWave'/Game/Equipment/Sounds/Rifle/Fire/Audio_3.Audio_3'",
	"/Script/Engine.SoundWave'/Game/Equipment/Sounds/Rifle/Fire/Audio_4.Audio_4'",
	"/Script/Engine.SoundWave'/Game/Equipment/Sounds/Rifle/Fire/Audio_5.Audio_5'",
	"/Script/Engine.SoundWave'/Game/Equipment/Sounds/Rifle/Fire/Audio_6.Audio_6'",
	"/Script/Engine.SoundWave'/Game/Equipment/Sounds/Rifle/Fire/Audio_7.Audio_7'",
	"/Script/Engine.SoundWave'/Game/Equipment/Sounds/Rifle/Fire/Audio_8.Audio_8'",
	"/Script/Engine.SoundWave'/Game/Equipment/Sounds/Rifle/Fire/Audio_9.Audio_9'"};

FString UGunFireSoundComponent::PistolGunFireSounds[4] = {
	"/Script/Engine.SoundWave'/Game/Equipment/Sounds/Pistol/Fire/Audio_10.Audio_10'",
	"/Script/Engine.SoundWave'/Game/Equipment/Sounds/Pistol/Fire/Audio_11.Audio_11'",
	"/Script/Engine.SoundWave'/Game/Equipment/Sounds/Pistol/Fire/Audio_12.Audio_12'",
	"/Script/Engine.SoundWave'/Game/Equipment/Sounds/Pistol/Fire/Audio_13.Audio_13'"};

// Sets default values for this component's properties
UGunFireSoundComponent::UGunFireSoundComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// RifleGunFireSounds[0] = "/Script/Engine.SoundWave'/Game/Equipment/Sounds/Rifle/Fire/Audio_1.Audio_1'";
	// RifleGunFireSounds[1] = "/Script/Engine.SoundWave'/Game/Equipment/Sounds/Rifle/Fire/Audio_2.Audio_2'";
	// RifleGunFireSounds[2] = "/Script/Engine.SoundWave'/Game/Equipment/Sounds/Rifle/Fire/Audio_3.Audio_3'";
	// RifleGunFireSounds[3] = "/Script/Engine.SoundWave'/Game/Equipment/Sounds/Rifle/Fire/Audio_4.Audio_4'";
	// RifleGunFireSounds[4] = "/Script/Engine.SoundWave'/Game/Equipment/Sounds/Rifle/Fire/Audio_5.Audio_5'";
	// RifleGunFireSounds[5] = "/Script/Engine.SoundWave'/Game/Equipment/Sounds/Rifle/Fire/Audio_6.Audio_6'";
	// RifleGunFireSounds[6] = "/Script/Engine.SoundWave'/Game/Equipment/Sounds/Rifle/Fire/Audio_7.Audio_7'";
	// RifleGunFireSounds[7] = "/Script/Engine.SoundWave'/Game/Equipment/Sounds/Rifle/Fire/Audio_8.Audio_8'";
	// RifleGunFireSounds[8] = "/Script/Engine.SoundWave'/Game/Equipment/Sounds/Rifle/Fire/Audio_9.Audio_9'";

	// PistolGunFireSounds[0] = "/Script/Engine.SoundWave'/Game/Equipment/Sounds/Pistol/Fire/Audio_10.Audio_10'";
	// PistolGunFireSounds[1] = "/Script/Engine.SoundWave'/Game/Equipment/Sounds/Pistol/Fire/Audio_11.Audio_11'";
	// PistolGunFireSounds[2] = "/Script/Engine.SoundWave'/Game/Equipment/Sounds/Pistol/Fire/Audio_12.Audio_12'";
	// PistolGunFireSounds[3] = "/Script/Engine.SoundWave'/Game/Equipment/Sounds/Pistol/Fire/Audio_13.Audio_13'";
}

void UGunFireSoundComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGunFireSoundComponent::MakeGunFireSound(EGun gunType, FVector location)
{
	FString randomSound;
	switch (gunType)
	{
	case EGun::G_PeaRifle:
		randomSound = RifleGunFireSounds[FMath::RandRange(0, 8)];
		break;
	case EGun::G_UrfGun:
		randomSound = PistolGunFireSounds[FMath::RandRange(0, 3)];
		break;
	}

	auto sound = USvUtilities::GetSoundWave(randomSound);
	if (sound)
	{
		auto actorLocation = GetOwner()->GetActorLocation();
		UGameplayStatics::SpawnSoundAtLocation(this, sound, GetOwner()->GetActorLocation());

		TestEchoes(actorLocation += FVector(1000, 1000, 0), sound);
		TestEchoes(actorLocation += FVector(1000, -1000, 0), sound);
		TestEchoes(actorLocation += FVector(-1000, 1000, 0), sound);
		TestEchoes(actorLocation += FVector(-1000, -1000, 0), sound);

		TestEchoes(actorLocation += FVector(-1000, 0, 0), sound);
		TestEchoes(actorLocation += FVector(1000, 0, 0), sound);
		TestEchoes(actorLocation += FVector(0, -1000, 0), sound);
		TestEchoes(actorLocation += FVector(0, 1000, 0), sound);
	}
}

void UGunFireSoundComponent::TestEchoes(FVector echoLoc, USoundWave *sound)
{
	FHitResult hit;
	auto world = GetOwner()->GetWorld();
	auto initialLoc = GetOwner()->GetActorLocation();
	world->LineTraceSingleByChannel(hit, initialLoc, echoLoc, USvUtilities::GetEnvironmentChannel());

	if (hit.GetActor())
	{
		// found echo
		auto distance = FVector::Dist(hit.Location, initialLoc);
		float distanceAsTime = ((float)distance / 1000) / 2;

		FTimerHandle handle;
		UDebugMessages::LogDisplay(this, "setting timer for echo in " + FString::SanitizeFloat(distanceAsTime));
		world->GetTimerManager().SetTimer(
			handle,
			[this, sound]()
			{
				UDebugMessages::LogDisplay(this, "played echo");
				UGameplayStatics::SpawnSoundAtLocation(this, sound, GetOwner()->GetActorLocation(), FRotator::ZeroRotator, .2f);
			},
			distanceAsTime, false);
	}
}