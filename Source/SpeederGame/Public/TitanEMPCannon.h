// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehicleAttachment.h"
#include "TitanEMPCannon.generated.h"

/**
 * 
 */
UCLASS()
class SPEEDERGAME_API UTitanEMPCannon : public UVehicleAttachment
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadOnly)
	bool m_WasFired = false;
	UPROPERTY(BlueprintReadOnly)
	bool m_WasDetonated = false;

	UPROPERTY(EditAnywhere,Category = Stats,meta = (AllowPrivateAccess))
	float m_EMPChargeTime = 200.0f;
	UPROPERTY(BlueprintReadOnly)
	float m_CurrentEMPChargeTime = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	float m_ChargePercentage = 0.0f;

	void ChargeEMP(float a_DeltaTime);
};
