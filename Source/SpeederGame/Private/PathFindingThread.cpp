// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFindingThread.h"

PathFindingThread::PathFindingThread(AStar* a_Pathfinder, FIntVector3 a_Startpoint, FIntVector3 a_Endpoint)
{
	m_Startpoint = a_Startpoint;
	m_Endpoint = a_Endpoint;
	m_Pathfinder = a_Pathfinder;
	m_Thread = FRunnableThread::Create(this, TEXT("MyThread"));
}

PathFindingThread::~PathFindingThread()
{
}

bool PathFindingThread::Init()
{
	return FRunnable::Init();
}

uint32 PathFindingThread::Run()
{
	//the thread calculates one path and then terminates itself just like a sort fo task 
	m_FinishedPath = m_Pathfinder->CalculatePath(m_Startpoint, m_Endpoint);
	m_FinishedTask = true;
	return 0;
}

void PathFindingThread::Exit()
{
	FRunnable::Exit();
	
}

void PathFindingThread::Stop()
{
	FRunnable::Stop();
	m_Pathfinder->m_StopCalculation = true;
	m_Thread->WaitForCompletion();

}
