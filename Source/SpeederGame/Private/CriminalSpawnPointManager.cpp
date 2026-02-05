// Fill out your copyright notice in the Description page of Project Settings.


#include "CriminalSpawnPointManager.h"

// Sets default values
ACriminalSpawnPointManager::ACriminalSpawnPointManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(m_DefaultRoot);


	m_Spawn0 = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn0"));
	m_Spawn0->SetupAttachment(RootComponent);

	m_Spawn1 = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn1"));
	m_Spawn1->SetupAttachment(RootComponent);

	m_Spawn2 = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn2"));
	m_Spawn2->SetupAttachment(RootComponent);

	m_Spawn3 = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn3"));
	m_Spawn3->SetupAttachment(RootComponent);

	m_Spawn4 = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn4"));
	m_Spawn4->SetupAttachment(RootComponent);

	m_Spawn5 = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn5"));
	m_Spawn5->SetupAttachment(RootComponent);

	m_Spawn6 = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn6"));
	m_Spawn6->SetupAttachment(RootComponent);

	

}

// Called when the game starts or when spawned
void ACriminalSpawnPointManager::BeginPlay()
{
	Super::BeginPlay();
	
	InitList();
	
}

// Called every frame
void ACriminalSpawnPointManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACriminalSpawnPointManager::ShowSpawnPositions()
{
	if(m_SpawnPositions.IsEmpty()) InitList();
	for (FVector Pos : m_SpawnPositions)
	{
		DrawDebugLine(GetWorld(), Pos, FVector(Pos.X, Pos.Y, Pos.Z + 300000.0f), FColor::Red, false, 30.0f);
	}

}

void ACriminalSpawnPointManager::InitList()
{
	m_SpawnPositions.Add(m_Spawn0->GetComponentLocation());
	m_SpawnPositions.Add(m_Spawn1->GetComponentLocation());
	m_SpawnPositions.Add(m_Spawn2->GetComponentLocation());
	m_SpawnPositions.Add(m_Spawn3->GetComponentLocation());
	m_SpawnPositions.Add(m_Spawn4->GetComponentLocation());
	m_SpawnPositions.Add(m_Spawn5->GetComponentLocation());
	m_SpawnPositions.Add(m_Spawn6->GetComponentLocation());

}

