// Fill out your copyright notice in the Description page of Project Settings.


#include "TrafficManager.h"


// Sets default values
ATrafficManager::ATrafficManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
}

// Called when the game starts or when spawned
void ATrafficManager::BeginPlay()
{
	Super::BeginPlay();
	//THIS WAS THE SAD TALE OF TRYING TO OPTIMIZE EVERYTHING WITH MULTITHREADING....

	/*TArray<AActor*> m_StreetActors;
	TSubclassOf<AStreetSpline> classToFind;
	classToFind = AStreetSpline::StaticClass();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, m_StreetActors);
	for (AActor* StreetActor : m_StreetActors)
	{
		m_Streets.Add(Cast<AStreetSpline>(StreetActor));
	}
	m_TrafficThread = new TrafficManageThread(m_Streets);*/
}

// Called every frame
void ATrafficManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

void ATrafficManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//if (m_TrafficThread != nullptr)
	//{
	//	m_TrafficThread->Stop();
	//	//m_Thread->m_Thread->Kill();
	//	delete m_TrafficThread;
	//	m_TrafficThread = nullptr;
	//}
}

