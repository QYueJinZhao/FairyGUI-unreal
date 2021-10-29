
#pragma once
#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "UI/GObject.h"
#include "Types/SlateEnums.h"
#include <vector>
#include <map>
class IDetailCategoryBuilder;
class IPropertyHandle;
class IDetailLayoutBuilder;


class FFUIDisplayObjectCustomization : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<class IDetailCustomization> MakeInstance()
	{
		return MakeShareable(new FFUIDisplayObjectCustomization());
	}

	FFUIDisplayObjectCustomization() {}
	virtual ~FFUIDisplayObjectCustomization(){}

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
	void AddText(IDetailCategoryBuilder& builder,  FString name,UGObject* obj, bool bEditable);
	void AddVector(IDetailCategoryBuilder& builder,  FString name,UGObject *obj, int nInput = 3);
	void AddCheckbox(IDetailCategoryBuilder& builder, FString name, UGObject* obj);

	TOptional<float> GetXYZ(FString name, UGObject* obj, int nIndex) const;
	void ChangeXYZ(float newValue, FString name, UGObject* obj, int nIndex);
	void CommitXYZ(float newValue, ETextCommit::Type textCommitType, FString name, UGObject* obj, int nIndex);

	FString GetText(FString name, UGObject* obj);
	void OnTextCommitted(const FText& newVal, ETextCommit::Type commitType, FString name,  UGObject* obj);
	void OnCheckStateChanged(ECheckBoxState checkBoxState, FString name, UGObject* obj);

};


DECLARE_DELEGATE_RetVal_OneParam(float, FOnRouteEvent, UGObject*);
DECLARE_DELEGATE_TwoParams(FOnSetRouteEvent, UGObject*, float);

template<class T >
class RouteQueue {
public:
	RouteQueue& Push(T handler) {
		handlers.push_back(handler);
		return *this;
	}

	T& GetHandler(int nIndex) {
		if (nIndex < handlers.size())
			return handlers[nIndex];

		static T dummy;
		return dummy;
	}

private:
	std::vector<T> handlers;
};

typedef RouteQueue<FOnRouteEvent> RouteGetter;
typedef RouteQueue<FOnSetRouteEvent> RouteSetter;