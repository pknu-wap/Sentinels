// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UI/STPlayerUIComponent.h"
#include "Net/UnrealNetwork.h"

USTPlayerUIComponent::USTPlayerUIComponent()
	:bIsReady(false)
{
}

void USTPlayerUIComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USTPlayerUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USTPlayerUIComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USTPlayerUIComponent, bIsReady);
}

void USTPlayerUIComponent::ServerRPCChangebIsReady_Implementation(const bool value)
{
	SetbIsReady(value);

    for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
    {
		APlayerController* pc = it->Get();
		if (pc)
		{
			USTPlayerUIComponent* UIComp = pc->FindComponentByClass<USTPlayerUIComponent>();
			if (UIComp)
			{
				if (!UIComp->GetbIsReady())
				{
					UE_LOG(LogTemp, Warning, TEXT("Not Ready"));
					return;
				}
			}
		}
    }

	UE_LOG(LogTemp, Warning, TEXT("All Ready"));
	MulticastRPCCheckbIsReady();
}

void USTPlayerUIComponent::MulticastRPCCheckbIsReady_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("UI Layer Delete"));
}
