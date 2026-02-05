// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPEEDERGAME_API AGM_GameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	

public:
	
	UPROPERTY(EditAnywhere, Category = UI, meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> m_PlayerHUD;
	UPROPERTY()
	UUserWidget* m_PlayerHUDWidget;

	
};
