
#include "BaseDirectorGameType.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../../../Interfaces/SvChar.h"
#include "../../../../../Utilities/SvUtilities.h"
#include "../../../../../GameModes/Managers/CharacterManager.h"
#include "../../../Behaviours/AIMeleeRangeMove.h"

UBaseDirectorGameType::UBaseDirectorGameType(const FObjectInitializer &ObjectInitializer)
    : UObject(ObjectInitializer)
{
    _randomStream = FRandomStream(FDateTime::Now().GetTicks() ^ GFrameCounter ^ FPlatformTime::Cycles());
}

float UBaseDirectorGameType::GeneratePreassure()
{
    UDebugMessages::LogError(this, "this should not happen massive error");
    return 0.0f;
}
void UBaseDirectorGameType::SpawnRequiredEnemies()
{
    UDebugMessages::LogError(this, "this should not happen massive error");
}

void UBaseDirectorGameType::MoveThisCharTowardsPlayer(TScriptInterface<ISvChar> movableCharacter)
{
    auto characterManager = USvUtilities::GetGameModeCharacterManager(GetWorld());

    TArray<TScriptInterface<ISvChar>> foundPlayerCharacters;
    characterManager->GetCharacterListOfCharacterType(ECharacterControl::CC_Player, foundPlayerCharacters);

    if (foundPlayerCharacters.Num() == 0)
    {
        UDebugMessages::LogError(this, "no found player characters");
        return;
    }

    auto characterToMoveTo = foundPlayerCharacters[0];

    int iteration = 0;
    UBaseAIBehaviour *movementBehaviour = NewObject<UAIMeleeRangeMove>(this);
    movementBehaviour->SetEnemyAndCharacters(movableCharacter->GetAsActor(), foundPlayerCharacters);
    movementBehaviour->DoBehaviour();
    while (!movementBehaviour->GetCompletedBehaviourAndWaitIfNot(.1f))
    {
        iteration += 1;
        if (iteration > 6000)
        {
            UDebugMessages::LogError(this, "movement took longer than a minute?! big error");
            break;
        }
    }
}
void UBaseDirectorGameType::SpawnEnemyOfTypeAtLocation(ABaseCharacter *baseCharacter, FVector loc, bool spawnFromFloor)
{
    return UDebugMessages::LogError(this, "SpawnEnemyOfTypeAtLocation Not implemented");
}

bool UBaseDirectorGameType::IsWithinList(FVector loc, TArray<FVector> location)
{
    for (int i = 0; i < location.Num(); i++)
        if (location[i] == loc)
            return true;

    return false;
}

void UBaseDirectorGameType::SetPlayerAndEnemyCharacters()
{
    auto characterManager = USvUtilities::GetGameModeCharacterManager(GetWorld());
    characterManager->GetCharacterListOfCharacterType(ECharacterControl::CC_Player, PlayerCharacters);
    characterManager->GetCharacterListOfCharacterType(ECharacterControl::CC_AI, EnemyCharacters);
}
