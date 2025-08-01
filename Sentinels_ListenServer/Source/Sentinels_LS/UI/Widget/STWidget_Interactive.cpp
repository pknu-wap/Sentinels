// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/STWidget_Interactive.h"
#include "STStructs.h"
#include "Components/TextBlock.h"

void USTWidget_Interactive::SetInteractiveUI(const FItemStruct* ItemInfo)
{
	if (!ItemInfo)
		return;

	FText itemName = FText::FromString(ItemInfo->ItemName);
	if (!itemName.IsEmpty())
		TB_Name->SetText(itemName);
	else
		TB_Name->SetText(FText::GetEmpty());

	FText itemDescription = FText::FromString(ItemInfo->ItemDescription);
	if (!itemDescription.IsEmpty())
		TB_Description->SetText(itemDescription);
	else
		TB_Description->SetText(FText::GetEmpty());
	
}

void USTWidget_Interactive::ClearInteractiveUI()
{
	TB_Name->SetText(FText::GetEmpty());
	TB_Description->SetText(FText::GetEmpty());
}
