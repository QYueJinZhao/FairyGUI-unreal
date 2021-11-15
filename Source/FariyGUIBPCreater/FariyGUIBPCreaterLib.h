#pragma once

#include "CoreMinimal.h"
#include "FairyGUI.h"
#include "FariyGUIBPCreaterLib.generated.h"

// 不支持动态调整asset 里面的生命周期
UCLASS()
class FARIYGUIBPCREATER_API UFariyGUIBPCreaterLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "PyEditor")
	static UBlueprint* PyCreateBlueprint(FString InClsName, FString InBpFullName);

	UFUNCTION(BlueprintCallable, Category = "PyEditor")
	static void PyAddObjectMemberValue(UBlueprint* InBlueprint, FName NewValType, FName NewVarName);

	UFUNCTION(BlueprintCallable, Category = "PyEditor")
	static void PyAddStringMemberValue(UBlueprint* InBlueprint, const FName& VariableName, const FString& DefaultValue);

	UFUNCTION(BlueprintCallable, Category = "PyEditor")
	static void PyCompileSaveBluePrint(UBlueprint* InBp);

public:
	UFUNCTION(BlueprintCallable, Category = "PyEditor")
	static void PyLogErr(FString context);

	UFUNCTION(BlueprintCallable, Category = "PyEditor")
	static void PyLogTrace(FString context);
};
