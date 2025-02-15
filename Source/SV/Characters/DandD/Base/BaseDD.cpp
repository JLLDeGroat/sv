#include "BaseDD.h"

UBaseDD::UBaseDD(const FObjectInitializer &ObjectInitializer) : UObject(ObjectInitializer)
{
}
FString UBaseDD::GetDeviationOrDirectiveDescription() { return Description; }
FString UBaseDD::GetDeviationOrDirectiveTitle() { return Title; }
EDDType UBaseDD::GetDeviationOrDirective() { return DeviationOrDirective; }

bool UBaseDD::GetActivatedThisLevel() { return bActivatedThisLevel; }
bool UBaseDD::GetActivatedThisTurn() { return bActivatedThisTurn; }

void UBaseDD::ResetOnNewTurn() { bActivatedThisTurn = false; }