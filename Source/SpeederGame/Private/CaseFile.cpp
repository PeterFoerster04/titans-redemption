// Fill out your copyright notice in the Description page of Project Settings.
#include "CaseFile.h"
#include "Components/WidgetComponent.h"


// Sets default values
ACaseFile::ACaseFile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(m_DefaultRoot);

	m_CaseFileWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FileWidget"));
	m_CaseFileWidget->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void ACaseFile::BeginPlay()
{
	Super::BeginPlay();
	 
}

// Called every frame
void ACaseFile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACaseFile::SetCustomFileValues(float a_Reward, float a_Speed, float a_SpeedMultiply, int a_SpawnIndex)
{
	m_MaxSpeed = a_Speed;
	m_RewardAmount = a_Reward;
	m_CriminalSpeedMultiplier = a_SpeedMultiply;
	m_SpawnIndex = a_SpawnIndex;
}

