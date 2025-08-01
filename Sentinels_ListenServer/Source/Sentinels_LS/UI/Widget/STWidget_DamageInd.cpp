// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/STWidget_DamageInd.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void USTWidget_DamageInd::NativeConstruct()
{
	Super::NativeConstruct();

	TB_Damage.Add(TB_Damage_0);
	TB_Damage.Add(TB_Damage_1);
	TB_Damage.Add(TB_Damage_2);
}

void USTWidget_DamageInd::SetDamage(float Damage)
{
	if (TB_Damage[CurrentIdx])
		TB_Damage[CurrentIdx]->SetText(UKismetTextLibrary::Conv_FloatToText(Damage, ERoundingMode::HalfToEven));
}

void USTWidget_DamageInd::SetDamageTextColor(FSlateColor& InColor)
{
	if (TB_Damage[CurrentIdx])
		TB_Damage[CurrentIdx]->SetColorAndOpacity(InColor);
}
