// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/STUISubSystem.h"
#include "Components/UI/STPlayerUIComponent.h"

void USTUISubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USTUISubSystem::Deinitialize()
{
	Super::Deinitialize();
}

void USTUISubSystem::RegisterWidget(FGameplayTag WidgetTag, UUserWidget* Widget)
{
	UUserWidget** widget = WidgetMap.Find(WidgetTag);
	if (widget) return;

	WidgetMap.Add(WidgetTag, Widget);
}

void USTUISubSystem::UnRegisterWidget(FGameplayTag WidgetTag)
{
	UUserWidget** widget = WidgetMap.Find(WidgetTag);
	if (widget)
	{
		(*widget)->RemoveFromParent();
		WidgetMap.Remove(WidgetTag);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO WIDGET"));
	}
}

void USTUISubSystem::SetWidgetVisibility(FGameplayTag WidgetTag, ESlateVisibility Visibility)
{
	UUserWidget** widget = WidgetMap.Find(WidgetTag);
	if (widget)
	{
		(*widget)->SetVisibility(Visibility);

		if (Visibility == ESlateVisibility::SelfHitTestInvisible
			|| Visibility == ESlateVisibility::HitTestInvisible
			|| Visibility == ESlateVisibility::Visible)
		{
			(*widget)->SetFocus();
		}
	}

}

ESlateVisibility USTUISubSystem::GetWidgetVisibility(FGameplayTag WidgetTag)
{
	UUserWidget** widget = WidgetMap.Find(WidgetTag);
	if (widget)
	{
		return (*widget)->GetVisibility();
	}

	return ESlateVisibility::Collapsed;
}

UUserWidget* USTUISubSystem::GetWidget(FGameplayTag WidgetTag)
{
	UUserWidget** widget = WidgetMap.Find(WidgetTag);
	if (widget)
	{
		return (*widget);
	}

	return nullptr;
}
