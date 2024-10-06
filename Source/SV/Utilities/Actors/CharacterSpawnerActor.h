// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Enums/EEquipmentEnums.h"
#include "CharacterSpawnerActor.generated.h"

class ABaseCharacter;
class UStaticMeshComponent;

UCLASS()
class SV_API ACharacterSpawnerActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACharacterSpawnerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner Debug Options") TSubclassOf<ABaseCharacter> CharacterClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner Debug Options") EGun GunType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner Debug Options") EGun SecondaryGunType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner Debug Options") FString OverrideCharacterName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner Debug Options") UStaticMeshComponent* RootMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner Debug Options") int GrenadeAmount;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner Debug Options") int HealthKitAmount;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner Debug Options") int TakeImmediateDamage;

};
