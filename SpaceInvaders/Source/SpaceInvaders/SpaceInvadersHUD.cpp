// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "SpaceInvadersHUD.h"


ASpaceInvadersHUD::ASpaceInvadersHUD()
{
	// Last inn klassene fra blueprints
	FStringClassReference MainHUDWidgetRef(TEXT("/Game/SpaceInvadersHUD.SpaceInvadersHUD_C"));
	MainHUDWidgetTemplate = MainHUDWidgetRef.TryLoadClass<UUserWidget>();

	ConstructorHelpers::FObjectFinder<UFont> oFont(TEXT("Font'/Game/Fonts/Pixeled.Pixeled'"));

	OurFont = oFont.Object;
}

void ASpaceInvadersHUD::BeginPlay()
{
	Super::BeginPlay();

	// Last inn UUserWidget fra Class-templates
	LoadInGameHUD();

	MyController = GetWorld()->GetFirstPlayerController();

	if (!MainHUDWidget->IsVisible())
		MainHUDWidget->AddToViewport();
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

	if (!MainHUDWidget)
		return;

	if (GameOver)
	{
		DrawGameOverScreen();
	}
	else
	{
		ScoreWidget->SetText(FText::FromString(FString::FromInt(Score)));
		HighscoreWidget->SetText(FText::FromString(FString::FromInt(HighScore)));
		LivesWidget->SetText(FText::FromString(FString::FromInt(Lives)));
	}
}

void ASpaceInvadersHUD::DrawGameOverScreen()
{
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	DrawRect(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), 0, 0, ViewportSize.X, ViewportSize.Y);
	DrawText("Game Over", FLinearColor(1.0f, 0.0f, 0.0f, 1.0f), ViewportSize.X / 2 - 280, ViewportSize.Y / 2 - 50, OurFont, 4.0f, false);
	DrawText("Press F to Restart", FLinearColor(1.0f, 0.0f, 0.0f, 1.0f), ViewportSize.X / 2 - 180, ViewportSize.Y / 2 + 50, OurFont, 1.0f, false);
}

