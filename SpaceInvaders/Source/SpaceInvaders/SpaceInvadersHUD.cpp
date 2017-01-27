// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "SpaceInvadersHUD.h"


ASpaceInvadersHUD::ASpaceInvadersHUD()
{
	FStringClassReference MainHUDWidgetRef(TEXT("/Game/SpaceInvadersHUD.SpaceInvadersHUD_C"));
	MainHUDWidgetTemplate = MainHUDWidgetRef.TryLoadClass<UUserWidget>();

	FStringClassReference MainMenuHUDWidgetRef(TEXT("/Game/MainMenu.MainMenu_C"));
	MainMenuHUDWidgetTemplate = MainMenuHUDWidgetRef.TryLoadClass<UUserWidget>();

	FStringClassReference HighscoreMenuHUDWidgetRef(TEXT("/Game/HighScore.HighScore_C"));
	HighscoreHUDWidgetTemplate = HighscoreMenuHUDWidgetRef.TryLoadClass<UUserWidget>();

	FStringClassReference PauseMenuHUDWidgetRef(TEXT("/Game/PauseMenu.PauseMenu_C"));
	PauseMenuHUDWidgetTemplate = PauseMenuHUDWidgetRef.TryLoadClass<UUserWidget>();

	FStringClassReference GameOverHUDWidgetRef(TEXT("/Game/GameOver.GameOver_C"));
	GameOverHUDWidgetTemplate = GameOverHUDWidgetRef.TryLoadClass<UUserWidget>();
}

void ASpaceInvadersHUD::BeginPlay()
{
	Super::BeginPlay();

	LoadMainMenu();
	LoadHighscoreMenu();
	LoadInGameHUD();
	LoadPauseMenu();
	LoadGameOver();

	MyController = GetWorld()->GetFirstPlayerController();

	MyController->bShowMouseCursor = true;
	MyController->bEnableClickEvents = true;
	MyController->bEnableMouseOverEvents = true;
	
	MainMenu = true;
}

void ASpaceInvadersHUD::LoadMainMenu()
{
	if (MainMenuHUDWidgetTemplate)
	{
		MainMenuHUDWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), MainMenuHUDWidgetTemplate);
	}
}

void ASpaceInvadersHUD::LoadGameOver()
{
	if (GameOverHUDWidgetTemplate)
	{
		GameOverHUDWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), GameOverHUDWidgetTemplate);
	}
}

void ASpaceInvadersHUD::LoadPauseMenu()
{
	if (PauseMenuHUDWidgetTemplate)
	{
		PauseMenuHUDWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), PauseMenuHUDWidgetTemplate);
	}
}

void ASpaceInvadersHUD::LoadHighscoreMenu()
{
	if (HighscoreHUDWidgetTemplate)
	{
		HighscoreHUDWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), HighscoreHUDWidgetTemplate);
	}
}

void ASpaceInvadersHUD::LoadInGameHUD()
{
	if (MainHUDWidgetTemplate)
	{
		MainHUDWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), MainHUDWidgetTemplate);
	}

	ScoreWidget = Cast<UTextBlock>(MainHUDWidget->GetWidgetFromName("Score"));
	HighscoreWidget = Cast<UTextBlock>(MainHUDWidget->GetWidgetFromName("Highscore"));
	LivesWidget = Cast<UTextBlock>(MainHUDWidget->GetWidgetFromName("Lives"));
}

void ASpaceInvadersHUD::DrawHUD()
{
	Super::DrawHUD();

	if (MainMenu && !HighscoreMenu)
	{
		if(MainHUDWidget->GetIsVisible())
			MainHUDWidget->RemoveFromViewport();
		if (HighscoreHUDWidget->GetIsVisible())
			HighscoreHUDWidget->RemoveFromViewport();
		if(!MainMenuHUDWidget->GetIsVisible())
			MainMenuHUDWidget->AddToViewport();

		if (!MyController->bShowMouseCursor)
			MyController->bShowMouseCursor = true;
		if (!MyController->bEnableClickEvents)
			MyController->bEnableClickEvents = true;
		if (!MyController->bEnableMouseOverEvents)
			MyController->bEnableMouseOverEvents = true;
	}
	else if (!MainMenu && HighscoreMenu)
	{
		if (MainMenuHUDWidget->GetIsVisible())
			MainMenuHUDWidget->RemoveFromViewport();
		if (!HighscoreHUDWidget->GetIsVisible())
			HighscoreHUDWidget->AddToViewport();

		if (!MyController->bShowMouseCursor)
			MyController->bShowMouseCursor = true;
		if (!MyController->bEnableClickEvents)
			MyController->bEnableClickEvents = true;
		if (!MyController->bEnableMouseOverEvents)
			MyController->bEnableMouseOverEvents = true;
	}
	else if (!MainMenu && PauseMenu) 
	{
		if (MainHUDWidget->GetIsVisible())
			MainHUDWidget->RemoveFromViewport();
		if (!PauseMenuHUDWidget->GetIsVisible())
			PauseMenuHUDWidget->AddToViewport();

		if (!MyController->bShowMouseCursor)
			MyController->bShowMouseCursor = true;
		if (!MyController->bEnableClickEvents)
			MyController->bEnableClickEvents = true;
		if (!MyController->bEnableMouseOverEvents)
			MyController->bEnableMouseOverEvents = true;
	}
	else if (!MainMenu && GameOver)
	{
		if (MainHUDWidget->GetIsVisible())
			MainHUDWidget->RemoveFromViewport();
		if (!GameOverHUDWidget->GetIsVisible())
			GameOverHUDWidget->AddToViewport();

		if (!MyController->bShowMouseCursor)
			MyController->bShowMouseCursor = true;
		if (!MyController->bEnableClickEvents)
			MyController->bEnableClickEvents = true;
		if (!MyController->bEnableMouseOverEvents)
			MyController->bEnableMouseOverEvents = true;
	}
	else
	{
		if (PauseMenuHUDWidget->GetIsVisible())
			PauseMenuHUDWidget->RemoveFromViewport();

		if(MainMenuHUDWidget->GetIsVisible())
			MainMenuHUDWidget->RemoveFromViewport();

		if(!MainHUDWidget->GetIsVisible())
			MainHUDWidget->AddToViewport();

		ScoreWidget->SetText(FText::FromString(FString::FromInt(Score)));
		HighscoreWidget->SetText(FText::FromString(FString::FromInt(HighScore)));
		LivesWidget->SetText(FText::FromString(FString::FromInt(Lives)));

		if (MyController->bShowMouseCursor)
			MyController->bShowMouseCursor = false;
		if (MyController->bEnableClickEvents)
			MyController->bEnableClickEvents = false;
		if (MyController->bEnableMouseOverEvents)
			MyController->bEnableMouseOverEvents = false;
	}
}

