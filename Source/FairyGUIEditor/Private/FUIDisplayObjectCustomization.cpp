// Copyright Epic Games, Inc. All Rights Reserved.

#include "FUIDisplayObjectCustomization.h"
#include "DetailWidgetRow.h" 
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "Widgets/DeclarativeSyntaxSupport.h" 
#include "Widgets/Text/STextBlock.h" 
#include "SFreeVectorInputBox.h"

FString FFUIDisplayObjectCustomization::GetText(FString name, UGObject* obj)
{
	FString content = "";
	TArray<FStringFormatArg> args;
	if(name.Compare("Name") == 0)
	{
		args.Add(obj->GetClass()->GetName());
		args.Add(obj->ID);

		content = FString::Format(TEXT("{0}: {1}"), args);
	}
	if(name.Compare("Resource") == 0)
	{
		args.Add(obj->GetPackageName());
		args.Add(obj->GetResourceName());
		content = FString::Format(TEXT("{0}@{1}"), args);
	}
	if (name.Compare("Text") == 0)
	{
		content = obj->GetText();
	}
	if (name.Compare("Icon") == 0)
	{
		content = obj->GetIcon();
	}
	return content;
}
void FFUIDisplayObjectCustomization::OnTextCommitted(const FText& newVal, ETextCommit::Type commitType, FString name, UGObject* obj)
{
	if (name.Compare("Text") == 0)
	{
		obj->SetText(newVal.ToString());
	}
}
void FFUIDisplayObjectCustomization::AddText(IDetailCategoryBuilder & builder, FString name, UGObject* obj, bool bEditable) {

	TSharedPtr<SWidget> contentWidget;
	if(bEditable)
	{
		contentWidget = SNew(SEditableTextBox)
			.IsEnabled(true)
			.Font(FCoreStyle::Get().GetFontStyle("SmallFont"))
			.Text(FText::FromString(GetText(name, obj)))
			.OnTextCommitted(this, &FFUIDisplayObjectCustomization::OnTextCommitted, name, obj);
	}else
	{
		contentWidget = SNew(STextBlock).Text(FText::FromString(GetText(name, obj)));
	}
	builder.AddCustomRow(FText::FromString(name))
		.NameContent()
		[
			SNew(STextBlock).Text(FText::FromString(name))
		].ValueContent()
		.MaxDesiredWidth(250.0f)
		[
			contentWidget.ToSharedRef()
		];
}

void FFUIDisplayObjectCustomization::AddVector(IDetailCategoryBuilder& builder, FString name, UGObject* obj, int nInput)
{
	builder.AddCustomRow(FText::FromString(name))
		.NameContent()
		[
			SNew(STextBlock).Text(FText::FromString(name))
		]
	.ValueContent()
		.MaxDesiredWidth(250.0f)
		[
			SNew(SFreeVectorInputBox)
			.N(nInput)
		.bColorAxisLabels(nInput > 1)
		.AllowResponsiveLayout(true)
		.X(this, &FFUIDisplayObjectCustomization::GetXYZ, name, obj, 0)
		.Y(this, &FFUIDisplayObjectCustomization::GetXYZ, name, obj, 1)
		.Z(this, &FFUIDisplayObjectCustomization::GetXYZ, name, obj, 2)
		.OnXChanged(this, &FFUIDisplayObjectCustomization::ChangeXYZ, name, obj, 0)
		.OnYChanged(this, &FFUIDisplayObjectCustomization::ChangeXYZ, name, obj, 1)
		.OnZChanged(this, &FFUIDisplayObjectCustomization::ChangeXYZ, name, obj, 2)
		.OnXCommitted(this, &FFUIDisplayObjectCustomization::CommitXYZ, name, obj, 0)
		.OnYCommitted(this, &FFUIDisplayObjectCustomization::CommitXYZ, name, obj, 1)
		.OnZCommitted(this, &FFUIDisplayObjectCustomization::CommitXYZ, name, obj, 2)
		.AllowSpin(false)
		.SpinDelta(0.0025f)				
		];
}

void FFUIDisplayObjectCustomization::AddCheckbox(IDetailCategoryBuilder& builder, FString name, UGObject* obj)
{
	TSharedPtr<SWidget> contentWidget;
	if (name == "Visible") {		
		contentWidget = SNew(SCheckBox)
			.IsChecked(obj->IsVisible()? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
			.OnCheckStateChanged(this, &FFUIDisplayObjectCustomization::OnCheckStateChanged, name, obj);
	}
	
	builder.AddCustomRow(FText::FromString(name))
		.NameContent()
		[
			SNew(STextBlock).Text(FText::FromString(name))
		].ValueContent()
		.MaxDesiredWidth(250.0f)
		[			
			contentWidget.ToSharedRef()
		];
}


void FFUIDisplayObjectCustomization::OnCheckStateChanged(ECheckBoxState checkBoxState, FString name, UGObject* obj)
{
	if (name == "Visible") {
		obj->SetVisible(checkBoxState== ECheckBoxState::Checked);
	}
}


TOptional<float> FFUIDisplayObjectCustomization::GetXYZ(FString name, UGObject* obj, int nIndex) const
{
	static std::map<FString, RouteGetter> RouteQueueGetter;
	if (RouteQueueGetter.empty()) {
		RouteQueueGetter["Position"]
			.Push(FOnRouteEvent::CreateLambda([](UGObject* obj)->float {return obj->GetPosition().X; }))
			.Push(FOnRouteEvent::CreateLambda([](UGObject* obj)->float {return obj->GetPosition().Y; }));

		RouteQueueGetter["Rotation"]
			.Push(FOnRouteEvent::CreateLambda([](UGObject* obj)->float {return obj->GetRotation(); }));

		RouteQueueGetter["Scale"]
			.Push(FOnRouteEvent::CreateLambda([](UGObject* obj)->float {return obj->GetScale().X; }))
			.Push(FOnRouteEvent::CreateLambda([](UGObject* obj)->float {return obj->GetScale().Y; }));

		RouteQueueGetter["Skew"]
			.Push(FOnRouteEvent::CreateLambda([](UGObject* obj)->float {return obj->GetSkew().X; }))
			.Push(FOnRouteEvent::CreateLambda([](UGObject* obj)->float {return obj->GetSkew().Y; }));

		RouteQueueGetter["Size"]
			.Push(FOnRouteEvent::CreateLambda([](UGObject* obj)->float {return obj->GetSize().X; }))
			.Push(FOnRouteEvent::CreateLambda([](UGObject* obj)->float {return obj->GetSize().Y; }));

		RouteQueueGetter["Pivot"]
			.Push(FOnRouteEvent::CreateLambda([](UGObject* obj)->float {return obj->GetPivot().X; }))
			.Push(FOnRouteEvent::CreateLambda([](UGObject* obj)->float {return obj->GetPivot().Y; }));

	}
	
	auto it = RouteQueueGetter.find(TCHAR_TO_UTF8(*name));
	if (it != RouteQueueGetter.end()) {
		return it->second.GetHandler(nIndex).Execute(obj);
	}
	

	return 0;
}

void FFUIDisplayObjectCustomization::ChangeXYZ(float newValue, FString name, UGObject* obj, int nIndex)
{
	static std::map<FString, RouteSetter> RouteQueueSetter;
	if(RouteQueueSetter.empty())
	{
		RouteQueueSetter["Position"]
			.Push(FOnSetRouteEvent::CreateLambda([](UGObject* obj, float newVal) { obj->SetX(newVal); }))
			.Push(FOnSetRouteEvent::CreateLambda([](UGObject* obj, float newVal) { obj->SetY(newVal); }));
		RouteQueueSetter["Rotation"]
			.Push(FOnSetRouteEvent::CreateLambda([](UGObject* obj, float newVal) { obj->SetRotation(newVal); }));
		RouteQueueSetter["Scale"]
			.Push(FOnSetRouteEvent::CreateLambda([](UGObject* obj, float newVal) { obj->SetScaleX(newVal); }))
			.Push(FOnSetRouteEvent::CreateLambda([](UGObject* obj, float newVal) { obj->SetScaleY(newVal); }));
		RouteQueueSetter["Skew"]
			.Push(FOnSetRouteEvent::CreateLambda([](UGObject* obj, float newVal) {auto data = obj->GetSkew(); data.X = newVal; obj->SetSkew(data); }))
			.Push(FOnSetRouteEvent::CreateLambda([](UGObject* obj, float newVal) { auto data = obj->GetSkew(); data.Y = newVal; obj->SetSkew(data); }));
		RouteQueueSetter["Size"]
			.Push(FOnSetRouteEvent::CreateLambda([](UGObject* obj, float newVal) {auto data = obj->GetSize(); data.X = newVal; obj->SetSize(data); }))
			.Push(FOnSetRouteEvent::CreateLambda([](UGObject* obj, float newVal) { auto data = obj->GetSize(); data.Y = newVal; obj->SetSize(data); }));
		RouteQueueSetter["Pivot"]
			.Push(FOnSetRouteEvent::CreateLambda([](UGObject* obj, float newVal) {auto data = obj->GetPivot(); data.X = newVal; obj->SetPivot(data); }))
			.Push(FOnSetRouteEvent::CreateLambda([](UGObject* obj, float newVal) { auto data = obj->GetPivot(); data.Y = newVal; obj->SetPivot(data); }));

	}

	auto it = RouteQueueSetter.find(TCHAR_TO_UTF8(*name));
	if (it != RouteQueueSetter.end()) {
		it->second.GetHandler(nIndex).Execute(obj, newValue);
	}

}

void FFUIDisplayObjectCustomization::CommitXYZ(float newValue, ETextCommit::Type textCommitType, FString name, UGObject* obj, int nIndex)
{
	ChangeXYZ(newValue, name, obj, nIndex);
}



void FFUIDisplayObjectCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	const TArray< TWeakObjectPtr<UObject> >& SelectedObjects = DetailLayout.GetSelectedObjects();
	if (SelectedObjects.Num() <= 0) {
		return;
	}

	UObject* curObject = SelectedObjects[0].Get();

	IDetailCategoryBuilder& DisplyObjectCategory = DetailLayout.EditCategory("DisplyObject", FText::GetEmpty(), ECategoryPriority::Important);

	UGObject* obj = Cast<UGObject>(curObject);
	if (obj == nullptr) {
		return;
	}

	AddText(DisplyObjectCategory, TEXT("Name"), obj, false);

	auto resourceURL = obj->GetResourceURL();
	if (resourceURL != "") {
		AddText(DisplyObjectCategory, TEXT("Resource"), obj, false);
	}

	AddVector(DisplyObjectCategory, "Position", obj,2);
	AddVector(DisplyObjectCategory,  "Rotation", obj,        1);
	AddVector(DisplyObjectCategory,  "Scale",obj,2);
	AddVector(DisplyObjectCategory,  "Skew",obj,2);
	AddVector(DisplyObjectCategory,  "Size",obj,2);
	AddVector(DisplyObjectCategory, "Pivot", obj,2);
	AddText(DisplyObjectCategory, "Text", obj, true);
	AddText(DisplyObjectCategory, "Icon", obj, true);

	AddCheckbox(DisplyObjectCategory, "Visible", obj);
}
