// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameModeClass.h"
#include "GameFramework/HUD.h"
#include "SpaceInvadersHUD.generated.h"


/**
 * 
 */
UCLASS()
class SPACEINVADERS_API ASpaceInvadersHUD : public AHUD
{
	GENERATED_BODY()
public:
	ASpaceInvadersHUD();

	virtual void DrawHUD() override;

	void DrawText(UFont* TheFont, const FString& TheString, const float& X, const float& Y, const FLinearColor& TheColor, const float& TheScale,
				  bool DrawOutline = false, const FLinearColor OutlineColor = FLinearColor(0, 0, 0, 1));

	void SetMainGameActor(AGameModeClass* GameMode)
	{
		MainGameMode = GameMode;
	}

	UFont* MyFont;

	AGameModeClass* MainGameMode;

};
