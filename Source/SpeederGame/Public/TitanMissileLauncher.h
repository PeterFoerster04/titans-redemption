// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehicleAttachment.h"
#include "TitanMissileLauncher.generated.h"

/**
 * 
 */
UCLASS()
class SPEEDERGAME_API UTitanMissileLauncher : public UVehicleAttachment
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int m_MaxRocketAmount = 4;
	UPROPERTY(BlueprintReadOnly)
	int m_CurrentRocketAmount = 0;



	
};
