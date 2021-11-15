#pragma once

#include "CoreMinimal.h"
#include "FairyGUI.h"
#include "FairyGUIAdapter.generated.h"

UCLASS()
class FAIRYGUI_API UFairyGUIAdapter : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	UGComponent* View;
};
