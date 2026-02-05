// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CaseMission.h"
#include "GameFramework/SaveGame.h"
#include "SpeederSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SPEEDERGAME_API USpeederSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int m_PlayerMoney = 0;

	UPROPERTY(BlueprintReadWrite)
	bool m_RepairedStun = false;
	UPROPERTY(BlueprintReadWrite)
	bool m_RepairedMissileLauncher = false;
	UPROPERTY(BlueprintReadWrite)
	bool m_RepairedBooster = false;
	UPROPERTY(BlueprintReadWrite)
	bool m_RepairedEMP = false;

	UPROPERTY(BlueprintReadWrite)
	float m_MouseSens = 1.0f;
	UPROPERTY(BlueprintReadWrite)
	float m_Volume = 1.0f;


	UPROPERTY()
	TArray<ACaseMission*> m_AccomplishedMissions;
};
