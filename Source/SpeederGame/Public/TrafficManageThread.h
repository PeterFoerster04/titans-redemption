// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class AStreetSpline;

//THIS WAS THE SAD TALE OF TRYING TO OPTIMIZE EVERYTHING WITH MULTITHREADING....

class SPEEDERGAME_API TrafficManageThread :public FRunnable
{
public:
	TrafficManageThread(TArray<AStreetSpline*>& a_Streets);
	~TrafficManageThread();


	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	virtual void Stop() override;

	FRunnableThread* m_Thread;
	TArray<AStreetSpline*> m_Streets;
	bool m_EndedGame = false;
	bool m_Update = false;
	float m_OldDelta;
	float m_CurrentDelta;
};
