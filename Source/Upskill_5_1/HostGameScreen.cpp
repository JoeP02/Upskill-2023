// Fill out your copyright notice in the Description page of Project Settings.


#include "HostGameScreen.h"

bool UHostGameScreen::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	return true;
}
