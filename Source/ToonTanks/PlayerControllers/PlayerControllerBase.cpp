// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerControllerBase.h"

void APlayerControllerBase::SetPlayerEnabledState(bool bSetPlayerEnabled)
{
	if (bSetPlayerEnabled)
	{
		GetPawn()->EnableInput(this);
	}
	else
	{
		GetPawn()->DisableInput(this);
	}

	bShowMouseCursor = bSetPlayerEnabled;
}