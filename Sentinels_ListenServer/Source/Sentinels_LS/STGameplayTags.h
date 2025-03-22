// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FSTGameplayTags
{

public:
	static const FSTGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	///////////  Tags  ///////////

	////////// Character /////////

	//////////  Widget   /////////

	//////////  Mission  /////////



private:
	static FSTGameplayTags GameplayTags;
};