// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FairyGUIEditor.h"
#include "Editor.h"
#include "FUIDisplayObjectCustomization.h"
#include "PropertyEditorModule.h"
#include "DetailCustomizations.h"
#include "FairyGUI.h"

#define LOCTEXT_NAMESPACE "FFairyGUIEditorModule"

void FFairyGUIEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
	PropertyEditorModule.RegisterCustomClassLayout(UGObject::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FFUIDisplayObjectCustomization::MakeInstance));
	PropertyEditorModule.NotifyCustomizationModuleChanged();
}

void FFairyGUIEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomClassLayout(UGObject::StaticClass()->GetFName());
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFairyGUIEditorModule, FairyGUIEditor)