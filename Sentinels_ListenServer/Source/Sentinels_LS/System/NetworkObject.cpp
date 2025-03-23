// Fill out your copyright notice in the Description page of Project Settings.


#include "System/NetworkObject.h"

int32 UNetworkObject::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	auto* Owner = GetTypedOuter<AActor>();
	return Owner ? Owner->GetFunctionCallspace(Function, Stack) : FunctionCallspace::Local;
}

bool UNetworkObject::CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack)
{
    if (AActor* Owner = GetTypedOuter<AActor>())
    {
        UNetDriver* NetDriver = Owner->GetNetDriver();
        if (NetDriver)
        {
            NetDriver->ProcessRemoteFunction(Owner, Function, Parameters, OutParms, Stack, this);
            return true;
        }
    }

    return false;
}
