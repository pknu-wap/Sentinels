// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NetworkObject.generated.h"


UCLASS(Abstract, Blueprintable, BlueprintType)
class SENTINELS_LS_API UNetworkObject : public UObject
{
	GENERATED_BODY()

/*
	UObject Interface
*/
public:
	FORCEINLINE bool IsSupportedForNetworking() const override { return true; }
	
	int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	bool CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack) override;

};
