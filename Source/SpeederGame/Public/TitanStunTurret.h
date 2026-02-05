// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehicleAttachment.h"
#include "TitanStunTurret.generated.h"

/**
 * 
 */
UCLASS()
class SPEEDERGAME_API UTitanStunTurret : public UVehicleAttachment
{
	GENERATED_BODY()

public:
	int m_MaxStunShots = 3;
	UPROPERTY(BlueprintReadOnly)
	int m_StunShotsLeft = 0;
	float m_StunRechargeTime = 15.0f;
	UPROPERTY(BlueprintReadOnly)
	float m_CurrentStunRechargeTime = 0.0f;


	void RechargeStunTurret(float a_DeltaTime);
	
};
