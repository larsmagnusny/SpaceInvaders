// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "SpaceInvadersHUD.h"


ASpaceInvadersHUD::ASpaceInvadersHUD()
{
	UE_LOG(LogTemp, Warning, TEXT("HUD INITIALIZED..."));

	ConstructorHelpers::FObjectFinder<UFont> FontObject(TEXT("Font'/Game/Pixeled.Pixeled'"));

	if (FontObject.Object)
		MyFont = FontObject.Object;
}

void ASpaceInvadersHUD::DrawText(UFont* TheFont, const FString& TheString, const float& X, const float& Y, const FLinearColor& TheColor, const float& TheScale,
	bool DrawOutline, const FLinearColor OutlineColor) 
{	
	if (!Canvas) return;

	// Text and font
	FCanvasTextItem NewText(
		FVector2D(X, Y),
		FText::FromString(TheString),
		TheFont,
		TheColor
	);

	// Text Scale
	NewText.Scale.Set(TheScale, TheScale);

	// Outline gets this alpha from main color
	NewText.bOutlined = true;
	NewText.OutlineColor = OutlineColor;
	NewText.OutlineColor.A = TheColor.A * 2;

	// Draw
	Canvas->DrawItem(NewText);
}

void ASpaceInvadersHUD::DrawHUD()
{
	Super::DrawHUD();
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	FVector2D MadeFor = FVector2D(1065.f, 596.f);

	float scaleFactorX = ViewportSize.X / 1065;
	float scaleFactorY = ViewportSize.Y / 596.f;

	DrawText(MyFont, FString("SCORE"), 390 * scaleFactorX, 20 * scaleFactorY, FLinearColor(1.f, 1.f, 1.f, 0.7f), 2.5f, false);
	DrawText(MyFont, FString::FromInt(Score), 450 * scaleFactorX, 70 * scaleFactorY, FLinearColor(1.f, 1.f, 1.f, 0.7f), 2.5f, false);

	DrawText(MyFont, FString("HIGHSCORE"), 690 * scaleFactorX, 20 * scaleFactorY, FLinearColor(1.f, 1.f, 1.f, 0.7f), 2.5f, false);
	DrawText(MyFont, FString::FromInt(HighScore), 800 * scaleFactorX, 70 * scaleFactorY, FLinearColor(1.f, 1.f, 1.f, 0.7f), 2.5f, false);

	DrawText(MyFont, FString("Lives"), 150 * scaleFactorX, 20 * scaleFactorY, FLinearColor(1.f, 1.f, 1.f, 0.7f), 2.5f, false);
	DrawText(MyFont, FString::FromInt(Lives), 150 * scaleFactorX, 70 * scaleFactorY, FLinearColor(1.f, 1.f, 1.f, 0.7f), 2.5f, false);
}

