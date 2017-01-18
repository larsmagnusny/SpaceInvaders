// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "SpaceInvadersHUD.h"


ASpaceInvadersHUD::ASpaceInvadersHUD()
{
	UE_LOG(LogTemp, Warning, TEXT("HUD INITIALIZED..."));

	ConstructorHelpers::FObjectFinder<UFont> FontObject(TEXT("Font'/Game/Pixeled.Pixeled'"));

	if (FontObject.Object)
		MyFont = FontObject.Object;
	
	//MainGameMode = (AGameModeClass*)GetWorld()->GetAuthGameMode();
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
	DrawText(MyFont, FString("This is just a test"), 100, 100, FLinearColor(1.f, 1.f, 1.f, 1.f), 1.f, false);
}

