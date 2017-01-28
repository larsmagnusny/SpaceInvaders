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
	void LoadMainMenu();
	void LoadHighscoreMenu();
	void LoadPauseMenu();
	void LoadGameOver();

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

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void SetMainMenu(bool val)
	{
		MainMenu = val;
	}

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void SetHighScoreMenu(bool val)
	{
		HighscoreMenu = val;
	}

	bool GetHighScoreMenu()
	{
		return HighscoreMenu;
	}

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void SetPauseMenu(bool val)
	{
		PauseMenu = val;
	}

	bool GetPauseMenu()
	{
		return PauseMenu;
	}

	bool GetMainMenu()
	{
		return MainMenu;
	}

	// Lagrer poengsum, liv, highscore og hvilken meny vi er i
	int32 Score = 0;
	int32 HighScore = 0;
	int32 Lives = 2;
	bool GameOver = false;
	bool MainMenu = true;
	bool HighscoreMenu = false;
	bool PauseMenu = false;

	// Holder widgets
	UClass* MainHUDWidgetTemplate;
	UUserWidget* MainHUDWidget;

	UClass* MainMenuHUDWidgetTemplate;
	UUserWidget* MainMenuHUDWidget;

	UClass* GameOverHUDWidgetTemplate;
	UUserWidget* GameOverHUDWidget;

	UClass* PauseMenuHUDWidgetTemplate;
	UUserWidget* PauseMenuHUDWidget;

	UClass* HighscoreHUDWidgetTemplate;
	UUserWidget* HighscoreHUDWidget;
private:
	// For å oppdatere widget variabler
	UTextBlock* ScoreWidget = nullptr;
	UTextBlock* HighscoreWidget = nullptr;
	UTextBlock* LivesWidget = nullptr;

	APlayerController* MyController;
};
