// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MissionObject/Interactable/Rift/Rift.h"

void ARift::Interact()
{
	Delegate_MissionConditionUpdate.Broadcast(ObjectID, true);
}
