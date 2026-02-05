// Fill out your copyright notice in the Description page of Project Settings.


#include "CaseMission.h"
#include "Criminal.h"

// Sets default values
ACaseMission::ACaseMission()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

}

// Called when the game starts or when spawned
void ACaseMission::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void ACaseMission::Tick(float DeltaTime)
{
	//mission handles if it was failed by checking the criminals hp and the time
	Super::Tick(DeltaTime);
	if (m_WasActivated &&!m_MissionFailed &&!m_MissionWasAbsolved)
	{
		TickTimer(DeltaTime);
		if(m_CriminalToChase->m_CriminalStats->m_VehicleCurrentHP<=0)
		{
			m_MissionFailed = true;
		}
	}
}

//extra init function since u objects can not have constructor parameters
void ACaseMission::InitMission(ACaseFile* a_File, float a_TimeLimit, ACriminal* a_Criminal)
{
	m_File = a_File;
	m_TimeLimit = a_TimeLimit;
	m_CurrentTime = m_TimeLimit;
	m_CriminalToChase = a_Criminal;
	m_MissionReward = m_File->m_RewardAmount;
}

void ACaseMission::TickTimer(float a_DeltaTime)
{
	m_CurrentTime -= a_DeltaTime;
	if(m_CurrentTime<0)
	{
		m_CurrentTime = 0.0f;
		m_MissionFailed = true;
	}
}







