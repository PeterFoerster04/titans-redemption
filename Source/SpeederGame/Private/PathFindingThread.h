// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AStar.h"
#include "CoreMinimal.h"

/**
 * 
 */
class PathFindingThread:public FRunnable
{
public:
	PathFindingThread(AStar* a_Pathfinder, FIntVector3 a_Startpoint, FIntVector3 a_Endpoint);
	~PathFindingThread();


	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	virtual void Stop() override;
	
	FRunnableThread* m_Thread;
	TArray<FVector> m_FinishedPath;
	AStar* m_Pathfinder;

	bool m_FinishedTask = false;
private:

	
	FIntVector3 m_Startpoint;
	FIntVector3 m_Endpoint;

};
