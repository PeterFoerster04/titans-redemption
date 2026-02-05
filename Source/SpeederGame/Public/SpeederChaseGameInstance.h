// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpeederSaveGame.h"
#include "Engine/GameInstance.h"
#include "SpeederChaseGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class SPEEDERGAME_API USpeederChaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Saves, meta = (AllowPrivateAccess))
	USpeederSaveGame* m_Saves;

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void LoadSaves();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SaveSaves();
};
