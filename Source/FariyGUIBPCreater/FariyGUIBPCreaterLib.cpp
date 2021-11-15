#include "FariyGUIBPCreaterLib.h"
#include "Adapter/FairyGUIAdapterLib.h"
#include "Editor/UnrealEd/Public/PackageTools.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "ObjectTools.h"
#include "UnrealEd/Public/FileHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"


UBlueprint* UFariyGUIBPCreaterLib::PyCreateBlueprint(FString InClsName, FString InBpFullName)
{
	if (!InBpFullName.StartsWith(TEXT("/"))) {
		PyLogErr("path must start with a /   :" + InBpFullName);
		return nullptr;
	}

	int32 Index = 0;
	InBpFullName.FindLastChar('/', Index);
	FString BpName = InBpFullName.Right(InBpFullName.Len() - Index - 1);
	if (BpName.Len() < 1) {
		PyLogErr("invalid blueprint name :" + InBpFullName);
		return nullptr;
	}

	UClass* InCls = UFairyGUIAdapterLib::FindFairyGUIAdapterClass(*InClsName);
	UClass* BpCls = UObject::StaticClass();
	if (InCls != nullptr) {

		BpCls = InCls;
	}
	UPackage* outer = CreatePackage(*InBpFullName);
	if (!outer)
	{
		PyLogErr("unable to create package :" + InBpFullName);
		return nullptr;
	}

	TArray<UPackage*> TopLevelPackages;
	TopLevelPackages.Add(outer);
	if (!PackageTools::HandleFullyLoadingPackages(TopLevelPackages, FText::FromString("Create a new object"))) {
		PyLogErr("unable to fully load package: " + InBpFullName);
		return nullptr;
	}

	auto OldBp = FindObject<UBlueprint>(outer, *BpName);
	if (OldBp != nullptr)
	{
		//PyLogErr("there is already a Blueprint with this name: " + BpName);
		return OldBp;
	}

	UBlueprint* bp = FKismetEditorUtilities::CreateBlueprint(BpCls, outer, *BpName, EBlueprintType::BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());
	if (!bp)
	{
		PyLogErr("unable to create Blueprint:" + InBpFullName);
		return nullptr;
	}

	FAssetRegistryModule::AssetCreated(bp);
	outer->MarkPackageDirty();
	return bp;
}

void UFariyGUIBPCreaterLib::PyAddObjectMemberValue(UBlueprint* InBlueprint, FName NewValType, FName NewVarName)
{
	if (InBlueprint == nullptr) {
		return;
	}

	UClass* InCls = UFairyGUIAdapterLib::FindFairyGUIAdapterClass(*NewValType.ToString());
	if (InCls == nullptr) {
		return;
	}
	FEdGraphPinType PinType(UEdGraphSchema_K2::PC_Object, NAME_None, InCls,
		EPinContainerType::None, false, FEdGraphTerminalType());

	if (FBlueprintEditorUtils::AddMemberVariable(InBlueprint, NewVarName, PinType))
	{
		for (FBPVariableDescription& NewVariable : InBlueprint->NewVariables)
		{
			if (NewVariable.VarName == NewVarName) {
				if (NewValType.ToString().StartsWith("UI_")) {
					NewVariable.SetMetaData("Category", TEXT("UI"));
				}
				else {
					NewVariable.SetMetaData("Category", NewValType.ToString());
				}
			}
		}

		return;
	}
}

void UFariyGUIBPCreaterLib::PyAddStringMemberValue(UBlueprint* InBlueprint, const FName& VariableName, const FString& DefaultValue)
{
	if (InBlueprint == nullptr) {
		return;
	}
	FEdGraphPinType StringPinType(UEdGraphSchema_K2::PC_String, NAME_None, nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
	FBlueprintEditorUtils::AddMemberVariable(InBlueprint, VariableName, StringPinType, DefaultValue);

}

void UFariyGUIBPCreaterLib::PyCompileSaveBluePrint(UBlueprint* InBp)
{
	if (InBlueprint == nullptr) {
		return;
	}
	FBlueprintEditorUtils::MarkBlueprintAsModified(InBlueprint);
	FKismetEditorUtilities::CompileBlueprint(InBlueprint);

	FString resName;
	StaticEnum<EBlueprintCompileMode>()->GetValueAsString(InBlueprint->Status, resName);
	PyLogTrace(FString(TEXT("PyCompileSaveBluePrint ")) + GetBPName(InBlueprint) + resName);

	UPackage* Package = Cast<UPackage>(InBlueprint->GetOuter());
	if (Package != nullptr) {
		TArray<UPackage*> PackagesToSave;
		PackagesToSave.Add(Package);
		FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, false, false);
	}
}

void UFariyGUIBPCreaterLib::PyLogErr(FString context)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *context);
}

void UFariyGUIBPCreaterLib::PyLogTrace(FString context)
{
	UE_LOG(LogTemp, Trace, TEXT("%s"), *context);
}
