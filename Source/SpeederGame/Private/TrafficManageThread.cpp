// Fill out your copyright notice in the Description page of Project Settings.


#include "TrafficManageThread.h"
#include "StreetSpline.h"


//THIS WAS THE SAD TALE OF TRYING TO OPTIMIZE EVERYTHING WITH MULTITHREADING....

TrafficManageThread::TrafficManageThread(TArray<AStreetSpline*>& a_Streets)
{
	m_Streets = a_Streets;
	
	m_Thread = FRunnableThread::Create(this, TEXT("MyThread"));
}

TrafficManageThread::~TrafficManageThread()
{
}

bool TrafficManageThread::Init()
{
	return FRunnable::Init();
}

uint32 TrafficManageThread::Run()
{
	//THIS WAS THE SAD TALE OF TRYING TO OPTIMIZE EVERYTHING WITH MULTITHREADING....
	//while(!m_EndedGame)
	//{
	//	//UE_LOG(LogTemp, Display, TEXT("SplineLenInArray: %f"), m_Streets[0]->m_Spline->GetSplineLength());
	//	//UE_LOG(LogTemp,Display,TEXT("This is the Traffic Thread dude"))

	//	if(!m_Update)
	//	{
	//		m_Streets[0]->UpdateVehicles(0.16f);
	//	}
	//}
	return 0;
}

void TrafficManageThread::Exit()
{
	FRunnable::Exit();
}

void TrafficManageThread::Stop()
{
	FRunnable::Stop();

	m_EndedGame = true;
	m_Thread->WaitForCompletion();
}
