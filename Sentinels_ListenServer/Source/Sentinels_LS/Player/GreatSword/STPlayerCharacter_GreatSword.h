// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/STPlayerCharacter.h"
#include "STPlayerCharacter_GreatSword.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API ASTPlayerCharacter_GreatSword : public ASTPlayerCharacter
{
	GENERATED_BODY()
	

protected:
	virtual void Skill_Q_Pressed() override;
	virtual void Skill_Q_Pressed_Server_Implementation() override;
	virtual void Skill_Q_Pressed_Multicast_Implementation() override;

	/*virtual void Skill_W_Pressed() override;
	virtual void Skill_W_Pressed_Server_Implementation() override;
	virtual void Skill_W_Pressed_Multicast_Implementation() override;

	virtual void Skill_E_Pressed() override;
	virtual void Skill_E_Pressed_Server_Implementation() override;
	virtual void Skill_E_Pressed_Multicast_Implementation() override;

	virtual void Skill_R_Pressed() override;
	virtual void Skill_R_Pressed_Server_Implementation() override;
	virtual void Skill_R_Pressed_Multicast_Implementation() override;*/
};
