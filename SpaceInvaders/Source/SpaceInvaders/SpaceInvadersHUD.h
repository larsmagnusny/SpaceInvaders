// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"		// Vi må kunne bruker UUserWidget
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

	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

	// Last inn blueprint widgets
	void LoadInGameHUD();

	FORCEINLINE void DrawJoyRect(
		float X, float Y,
		float Width, float Height,
		const FLinearColor& Color
	)
	{
		if (!Canvas) return;
		//

		FCanvasTileItem RectItem(
			FVector2D(X, Y),
			FVector2D(Width, Height),
			Color
		);

		RectItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(RectItem);
	}

	FORCEINLINE void DrawJoyText(
		UFont*	TheFont,
		const FString& TheStr,
		const float& X, const float& Y,
		const FLinearColor& TheColor,
		const float& TheScale,
		bool DrawOutline = false,
		const FLinearColor OutlineColor = FLinearColor(0, 0, 0, 1)
	) {
		if (!Canvas) return;
		//

		//Text and Font
		FCanvasTextItem NewText(
			FVector2D(X, Y),
			FText::FromString(TheStr),
			TheFont,
			TheColor
		);

		//Text Scale
		NewText.Scale.Set(TheScale, TheScale);

		//Outline gets its alpha from the main color
		NewText.bOutlined = true;
		NewText.OutlineColor = OutlineColor;
		NewText.OutlineColor.A = TheColor.A * 2;

		//Draw
		Canvas->DrawItem(NewText);
	}

	void DrawGameOverScreen();

	// Getters and setters
	void SetScore(int32 n) 
	{
		Score = n; 
	}

	UFUNCTION(BlueprintCallable, Category = "Menu")
	int32 GetScore() 
	{
		return Score; 
	}

	void SetHighScore(int32 n)
	{
		HighScore = n; 
	}

	int32 GetHighScore()
	{ 
		return HighScore; 
	}

	int32 GetLives()
	{
		return Lives; 
	}

	void SetLives(int32 n) 
	{ 
		Lives = n;
	}

	void SetGameOver(bool Boolean) {
		GameOver = Boolean; 
	}

	bool GetGameOver() {
		return GameOver; 
	}

	// Lagrer poengsum, liv, highscore og hvilken meny vi er i
	int32 Score = 0;
	int32 HighScore = 0;
	int32 Lives = 2;
	bool GameOver = false;
	bool MainMenu = true;
	bool HighscoreMenu = false;
	bool PauseMenu = false;

	bool collisionEventWasFired = false;

	// Holder widgets
	UClass* MainHUDWidgetTemplate = nullptr;
	UUserWidget* MainHUDWidget = nullptr;

	UClass* GameOverHUDWidgetTemplate = nullptr;
	UUserWidget* GameOverHUDWidget = nullptr;

private:
	// For å oppdatere widget variabler
	UTextBlock* ScoreWidget = nullptr;
	UTextBlock* HighscoreWidget = nullptr;
	UTextBlock* LivesWidget = nullptr;

	APlayerController* MyController;

	UFont* OurFont = nullptr;
};
