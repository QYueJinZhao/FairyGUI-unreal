#include "FairyGUIAdapterLib.h"
#include "FairyGUIAdapter.h"
#include "CoreGlobals.h"
#include "UObject/UObjectGlobals.h"

static void LogErr(FString context)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *context);
}

#define LOG_CHCEK_PROPERTY(obj, Msg) if (obj == nullptr) {LogErr("BindFairyGui not find Property=" +PropertyName + Msg + PropertyClsName);  }
static void BindFairyGUI(UFairyGUIAdapter* Adapter, UGComponent* Component, UObject* WorldContextObject)
{
	Adapter->View = Component;
	for (TFieldIterator<FObjectProperty> PropIt(Adapter->GetClass()); PropIt; ++PropIt)
	{
		FObjectProperty* Property = *PropIt;

		FString PropertyName = Property->GetName();
		if (PropertyName == "View") {
			continue;
		}
		if (PropertyName.StartsWith("m_")) {
			PropertyName = PropertyName.Mid(2);
		}

		uint8* AdapterAddr = Property->ContainerPtrToValuePtr<uint8>(Adapter);
		FString PropertyClsName = Adapter->GetClass()->GetName();
		if (Property->PropertyClass->IsChildOf(UGComponent::StaticClass())) {
			auto obj = Adapter->View->GetChild(PropertyName, Property->PropertyClass);
			LOG_CHCEK_PROPERTY(obj, "GetChild");
			Property->SetObjectPropertyValue(AdapterAddr, obj);
		}
		else if (Property->PropertyClass->IsChildOf(UGController::StaticClass())) {
			auto obj = Component->GetController(PropertyName);
			LOG_CHCEK_PROPERTY(obj, "GetController");
			Property->SetObjectPropertyValue(AdapterAddr, obj);
		}
		else if (Property->PropertyClass->IsChildOf(UTransition::StaticClass())) {
			auto obj = Component->GetTransition(PropertyName);
			LOG_CHCEK_PROPERTY(obj, "GetTransition");
			Property->SetObjectPropertyValue(AdapterAddr, obj);
		}
		else if (Property->PropertyClass->IsChildOf(UFairyGUIAdapter::StaticClass())) {
			auto obj = Adapter->View->GetChild(PropertyName, Property->PropertyClass);
			LOG_CHCEK_PROPERTY(obj, "GetChild");
			auto newobj = UFairyGUIAdapterLib::ConstructFairyGUI(Property->PropertyClass->GetFName(), Cast<UGComponent>(obj), WorldContextObject);
			LOG_CHCEK_PROPERTY(newobj, "FairyGUIAdapter");
			Property->SetObjectPropertyValue(AdapterAddr, newobj);
		}
	}
}


UFairyGUIAdapter* UFairyGUIAdapterLib::ConstructFairyGUI(FName InBriefClsName, UGComponent* Component, UObject* WorldContextObject)
{
	auto Class = FindFairyGUIAdapterClass(InBriefClsName.ToString());
	FString pakPath = FPaths::GetPath(Class->GetPathName());
	pakPath = "/FairyGUI-unreal/FairyRes/" + pakPath.Mid(15); // FairyRes 插件资源目录配置
	UUIPackage::AddPackage(*pakPath, WorldContextObject);

	UFairyGUIAdapter* Adapter = NewObject<UFairyGUIAdapter>(WorldContextObject, Class, NAME_None, RF_Transactional);
	if (Component == nullptr) {
		for (TFieldIterator<FProperty> PropIt(Class); PropIt; ++PropIt)
		{
			FProperty* Property = *PropIt;
			if (Property->GetName() == "URL")
			{
				FStrProperty* Value = Cast<FStrProperty>(*PropIt);
				FString TempStr;
				Value->ExportTextItem(TempStr, Value->ContainerPtrToValuePtr<void*>(Adapter), nullptr, nullptr, EPropertyPortFlags::PPF_None, nullptr);
				Component = Cast<UGComponent>(UUIPackage::CreateObjectFromURL(TempStr, WorldContextObject, UGComponent::StaticClass()));
			}
		}
	}
	if (Adapter == nullptr) {
		LogErr(TEXT("PyCreateFairyGui is not exist!") + InBriefClsName.ToString());
		return nullptr;
	}
	if (Component == nullptr)
	{
		LogErr(TEXT("Component is not exist!") + InBriefClsName.ToString());
		return nullptr;
	}

	BindFairyGUI(Adapter, Component, WorldContextObject);
	return Adapter;
}

UClass* UFairyGUIAdapterLib::FindFairyGUIAdapterClass(FString ClassName)
{
	UObject* ClassPackage = ANY_PACKAGE;

	if (!ClassName.EndsWith("_C")) {
		if (auto* Result = FindObject<UClass>(ClassPackage, *(ClassName + "_C")))
			return Result;
	}
	if (auto* Result = FindObject<UClass>(ClassPackage, *ClassName))
		return Result;

	LogErr(TEXT("FindFairyGUIAdapterClass is not exist!") + ClassName);
	return nullptr;
}

