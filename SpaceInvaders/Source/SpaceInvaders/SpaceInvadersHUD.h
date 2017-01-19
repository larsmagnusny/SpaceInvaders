// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
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
	void DrawLine(
		const FVector2D& Start,
		const FVector2D& End,
		const FLinearColor& TheColor,
		const float& Thick
	);

	void DrawRect(
		float X, float Y,
		float Width, float Height,
		const FLinearColor& Color
	);


	UFont* MyFont;

	void SetScore(int32 n) { Score = n; }

	int32 GetScore() { return Score; }

	void SetHighScore(int32 n) { HighScore = n; }
	int32 GetHighScore() { return HighScore; }

	int32 GetLives() { return Lives; }

	void SetLives(int32 n) { Lives = n; }

	void SetGameOver(bool Boolean) { GameOver = Boolean; }
	bool GetGameOver() { return GameOver; }

	int32 Score = 0;
	int32 HighScore = 0;
	int32 Lives = 2;

	bool GameOver = false;

private:
};
