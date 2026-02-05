// Fill out your copyright notice in the Description page of Project Settings.


#include "CriminalStats.h"

// Sets default values for this component's properties
UCriminalStats::UCriminalStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCriminalStats::BeginPlay()
{
	Super::BeginPlay();
	m_VehicleCurrentHP = m_VehicleMaxHP;
	m_VehicleCurrentShields = m_VehicleMaxShields;
	
	
}


// Called every frame
void UCriminalStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



