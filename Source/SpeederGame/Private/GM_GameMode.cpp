// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_GameMode.h"

#include "Blueprint/UserWidget.h"


void AGM_GameMode::BeginPlay()
{
	Super::BeginPlay();
	if (m_PlayerHUD != nullptr)
	{
		m_PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), m_PlayerHUD);
		m_PlayerHUDWidget->AddToViewport();
	}
}


