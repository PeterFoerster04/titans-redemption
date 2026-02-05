// Fill out your copyright notice in the Description page of Project Settings.


#include "TitanEMPCannon.h"

void UTitanEMPCannon::ChargeEMP(float a_DeltaTime)
{
	if (m_CurrentEMPChargeTime > 0)
	{
		m_CurrentEMPChargeTime -= a_DeltaTime;
		m_ChargePercentage = (1.0f -(m_CurrentEMPChargeTime / m_EMPChargeTime))* 100.0f;
	}
	else {

		m_ChargePercentage = 100.0f;
		
	}
}
