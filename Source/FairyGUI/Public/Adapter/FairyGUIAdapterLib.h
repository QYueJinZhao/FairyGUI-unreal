#pragma once

#include "CoreMinimal.h"
#include "FairyGUI.h"
#include "FairyGUIAdapterLib.generated.h"

// ��֧�ֶ�̬����asset �������������
UCLASS()
class FAIRYGUI_API UFairyGUIAdapterLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "FairyGUIAdapter", meta = (WorldContext = "WorldContextObject"))
	static UFairyGUIAdapter* ConstructFairyGUI(FName InBriefClsName, UGComponent* Component, UObject* WorldContextObject);

	static UClass* FindFairyGUIAdapterClass(FString ClassName);
};
