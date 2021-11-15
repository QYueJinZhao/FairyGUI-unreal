/*
static bool IsImplementationDesiredAsFunction(UBlueprint* InBlueprint, const UFunction* OverrideFunc)
{
	// If the original function was created in a parent blueprint, then prefer a BP function
	if (OverrideFunc)
	{
		FName OverrideName = *OverrideFunc->GetName();
		TSet<FName> GraphNames;
		FBlueprintEditorUtils::GetAllGraphNames(InBlueprint, GraphNames);
		for (const FName& Name : GraphNames)
		{
			if (Name == OverrideName)
			{
				return true;
			}
		}
	}

	// Otherwise, we would prefer an event
	return false;
}

UEdGraph* UPyEditorBlueprintLibrary::PyOverrideFunction(UBlueprint* InBlueprint, const FName& FunctionName)
{
	if (InBlueprint == nullptr) {
		return nullptr;
	}

	PyLogTrace(FString(TEXT("PyCreateNewFunctionGraph ")) + GetBPName(InBlueprint) + FunctionName.ToString());

	UClass* SuperClass = InBlueprint->GeneratedClass->GetSuperClass();
	UFunction* ParentFunction = SuperClass->FindFunctionByName(FunctionName);
	UFunction* Function = InBlueprint->GeneratedClass->FindFunctionByName(FunctionName, EIncludeSuperFlag::ExcludeSuper);
	if (ParentFunction)
	{
		UFunction* OverrideFunc = nullptr;
		UClass* const OverrideFuncClass = FBlueprintEditorUtils::GetOverrideFunctionClass(InBlueprint, FunctionName, &OverrideFunc);
		check(OverrideFunc);

		UEdGraph* EventGraph = FBlueprintEditorUtils::FindEventGraph(InBlueprint);

		if (UEdGraphSchema_K2::FunctionCanBePlacedAsEvent(OverrideFunc) && !IsImplementationDesiredAsFunction(InBlueprint, OverrideFunc) && EventGraph)
		{
			// Add to event graph
			FName EventName = OverrideFunc->GetFName();
			UK2Node_Event* ExistingNode = FBlueprintEditorUtils::FindOverrideForFunction(InBlueprint, OverrideFuncClass, EventName);

			if (!ExistingNode && !Function)
			{
				if (OverrideFuncClass == InBlueprint->GeneratedClass)
				{
					ExistingNode = FBlueprintEditorUtils::FindOverrideForFunction(InBlueprint, UObject::StaticClass(), EventName);
					if (ExistingNode && !ExistingNode->IsNodeEnabled())
					{
						EventGraph->Nodes.RemoveAll([&](UEdGraphNode* GraphNode) {return GraphNode == ExistingNode; });
						FBlueprintEditorUtils::RemoveNode(InBlueprint, ExistingNode);
					}
				}
				UK2Node_Event* NewEventNode = FEdGraphSchemaAction_K2NewNode::SpawnNode<UK2Node_Event>(
					EventGraph,
					EventGraph->GetGoodPlaceForNewNode(),
					EK2NewNodeFlags::SelectNewNode,
					[EventName, OverrideFuncClass](UK2Node_Event* NewInstance)
				{
					NewInstance->EventReference.SetExternalMember(EventName, OverrideFuncClass);
					NewInstance->bOverrideFunction = true;
				}
				);
			}
		}
		else
		{
			UEdGraph* const ExistingGraph = FindObject<UEdGraph>(InBlueprint, *FunctionName.ToString());
			if (!ExistingGraph)
			{
				InBlueprint->Modify();
				// Implement the function graph
				UEdGraph* const NewGraph = FBlueprintEditorUtils::CreateNewGraph(InBlueprint, FunctionName, UEdGraph::StaticClass(), UEdGraphSchema_K2::StaticClass());
				FBlueprintEditorUtils::AddFunctionGraph(InBlueprint, NewGraph,  false, OverrideFuncClass);
				NewGraph->Modify();
				return NewGraph;
			}
			return ExistingGraph;
		}

		return EventGraph;
	}
	return nullptr;
}

//Add BeginPlay event to graph
			UEdGraph* EventGraph = FBlueprintEditorUtils::FindEventGraph(BlueprintObject);
			PostBeginPlayEventNode = EditorBuildPromotionTestUtils::CreatePostBeginPlayEvent(BlueprintObject, EventGraph);
			Test->TestNotNull(TEXT("Event Being Play node added"), PostBeginPlayEventNode);
			if (!PostBeginPlayEventNode)
			{
				return true;
			}

			// Add string member variable
			FEdGraphPinType StringPinType(UEdGraphSchema_K2::PC_String, NAME_None, nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
			FBlueprintEditorUtils::AddMemberVariable(BlueprintObject, EditorBuildPromotionTestUtils::BlueprintStringVariableName, StringPinType);

			// Add print string node connected to the BeginPlay node; save it for use later
			UEdGraphPin* PlayThenPin = PostBeginPlayEventNode->FindPin(UEdGraphSchema_K2::PN_Then);
			PrintNode = EditorBuildPromotionTestUtils::AddPrintStringNode(BlueprintObject, EventGraph, PlayThenPin);
			Test->TestNotNull(TEXT("Print String node added"), PrintNode);


					FEdGraphPinType IntegerPinType(UEdGraphSchema_K2::PC_Int, NAME_None, nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
		FBlueprintEditorUtils::AddMemberVariable(InBlueprint, VariableName, IntegerPinType);


*/

//E:\Program Files\Epic Games\UE_4.26\Engine\Source\Editor\Kismet\Private\Tests\BlueprintEditorTests.cpp

/*
void UPyEditorBlueprintLibrary::PySetReturnValueByAttr(UBlueprint* InBlueprint, const FName FunctionName, const FName AttrName)
{
	UEdGraph* ExistingGraph = FindObject<UEdGraph>(InBlueprint, *FunctionName.ToString());
	if (ExistingGraph == nullptr) {
		return;
	}

	UEdGraphPin* ReturnValuePin = nullptr;
	for (auto i = 0; i < ExistingGraph->Nodes.Num(); i++) {
		ReturnValuePin = ExistingGraph->Nodes[i]->FindPin(UEdGraphSchema_K2::PN_ReturnValue);
		if (ReturnValuePin != nullptr) { break; }
	}

	if (ReturnValuePin == nullptr) {
		return;
	}

	auto GetNode = PyAddGetSetNode(InBlueprint, ExistingGraph, AttrName.ToString(), true);
	UEdGraphPin* GetExecPin = GetNode->FindPin(AttrName.ToString());
	GetExecPin->MakeLinkTo(ReturnValuePin);
}

void UPyEditorBlueprintLibrary::PySetReturnValue(UEdGraphNode* Node, const FName PinName, const FString& PinValue)
{
	if (Node == nullptr) {
		return;
	}
	UEdGraphPin* ReturnValuePin = Node->FindPin(UEdGraphSchema_K2::PN_ReturnValue);
	if (ReturnValuePin) {
		ReturnValuePin->DefaultValue = PinValue;
	}
}
void UPyEditorBlueprintLibrary::PySetPinDefaultObject(UEdGraphNode* Node, const FName PinName, UObject* PinObject) {
	if (Node == nullptr) {
		return;
	}
	UEdGraphPin* Pin = Node->FindPin(PinName);
	Pin->DefaultObject = PinObject;
}

*/