// Fill out your copyright notice in the Description page of Project Settings.


#include "PoliceStationUIManager.h"

#include "GM_GameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"

// Sets default values for this component's properties
UPoliceStationUIManager::UPoliceStationUIManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UPoliceStationUIManager::BeginPlay()
{
	Super::BeginPlay();
	m_World = GetWorld();
	
	
	
}


// Called every frame
void UPoliceStationUIManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(!m_GotPlayerHUD)
	{
		m_PlayerHUDWidget = Cast<AGM_GameMode>(UGameplayStatics::GetGameMode(m_World))->m_PlayerHUDWidget;

		m_GotPlayerHUD = true;
	}
	// ...
}
//these functions are called in blueprint
//just swapping ui widget around and setting the input modes

void UPoliceStationUIManager::SwapToGarageMenu()
{
	if (m_PlayerHUDWidget != nullptr)m_PlayerHUDWidget->RemoveFromParent();
	if (m_MissionUIWidget != nullptr)m_MissionUIWidget->RemoveFromParent();
	if (m_AdjustmentUIWidget != nullptr)m_AdjustmentUIWidget->RemoveFromParent();

	if (m_GarageUI != nullptr)
	{
		m_GarageUIWidget = CreateWidget<UUserWidget>(GetWorld(), m_GarageUI);
		m_GarageUIWidget->AddToViewport();
	}
	FInputModeUIOnly UIOnly;
	UGameplayStatics::GetPlayerController(m_World, 0)->SetShowMouseCursor(true);
	UGameplayStatics::GetPlayerController(m_World, 0)->SetInputMode(UIOnly);
	
}

void UPoliceStationUIManager::SwapToAdjustmentMenu()
{
	if (m_GarageUIWidget != nullptr)m_GarageUIWidget->RemoveFromParent();
	if (m_AdjustmentUI != nullptr)
	{
		m_AdjustmentUIWidget = CreateWidget<UUserWidget>(GetWorld(), m_AdjustmentUI);
		m_AdjustmentUIWidget->AddToViewport();
	}
	
	FInputModeUIOnly UIOnly;
	UGameplayStatics::GetPlayerController(m_World, 0)->SetShowMouseCursor(true);
	UGameplayStatics::GetPlayerController(m_World, 0)->SetInputMode(UIOnly);

}

void UPoliceStationUIManager::SwapToPlayerHUD()
{
	if (m_MissionUIWidget != nullptr)m_MissionUIWidget->RemoveFromParent();
	if(m_GarageUIWidget!= nullptr)m_GarageUIWidget->RemoveFromParent();
	if (m_PlayerHUD != nullptr)
	{
		m_PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), m_PlayerHUD);
		m_PlayerHUDWidget->AddToViewport();
	}
	FInputModeGameOnly GameOnly;
	UGameplayStatics::GetPlayerController(m_World, 0)->SetShowMouseCursor(false);
	UGameplayStatics::GetPlayerController(m_World, 0)->SetInputMode(GameOnly);
}

void UPoliceStationUIManager::SwapToMissionSelectMenu()
{
	if (m_GarageUIWidget != nullptr)m_GarageUIWidget->RemoveFromParent();
	if (m_PoliceMissionUI != nullptr)
	{
		m_MissionUIWidget = CreateWidget<UUserWidget>(GetWorld(), m_PoliceMissionUI);
		m_MissionUIWidget->AddToViewport();
	}

	FInputModeUIOnly UIOnly;
	UGameplayStatics::GetPlayerController(m_World, 0)->SetShowMouseCursor(true);
	UGameplayStatics::GetPlayerController(m_World, 0)->SetInputMode(UIOnly);
}

void UPoliceStationUIManager::ClearCurrentUI()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(m_World);

	
}



