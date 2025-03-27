// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UI/STUIComponent_Base.h"

// Sets default values for this component's properties
USTUIComponent_Base::USTUIComponent_Base()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USTUIComponent_Base::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USTUIComponent_Base::InitializeComponent()
{

}


// Called every frame
void USTUIComponent_Base::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

