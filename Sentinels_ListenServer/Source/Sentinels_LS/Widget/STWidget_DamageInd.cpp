// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/STWidget_DamageInd.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void USTWidget_DamageInd::SetDamage(float Damage)
{
	if (TB_Damage)
		TB_Damage->SetText(UKismetTextLibrary::Conv_FloatToText(Damage, ERoundingMode::HalfToEven));
}

void USTWidget_DamageInd::SetTextColor(FSlateColor& InColor)
{
	if (TB_Damage)
		TB_Damage->SetColorAndOpacity(InColor);
}
