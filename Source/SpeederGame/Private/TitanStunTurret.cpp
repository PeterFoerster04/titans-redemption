// Fill out your copyright notice in the Description page of Project Settings.


#include "TitanStunTurret.h"

void UTitanStunTurret::RechargeStunTurret(float a_DeltaTime)
{
	if (m_StunShotsLeft > 0)return;
	if (m_CurrentStunRechargeTime > 0)m_CurrentStunRechargeTime -= a_DeltaTime;
	else{
		m_StunShotsLeft = m_MaxStunShots;
		m_CurrentStunRechargeTime = m_StunRechargeTime;
	}
}
