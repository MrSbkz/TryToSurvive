#pragma once

#include "InteractiveActor.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractiveActor : public UInterface
{
    GENERATED_BODY()
};

class IInteractiveActor
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    // ReSharper disable once CppHiddenFunction
    void Interact();
};