// Fill out your copyright notice in the Description page of Project Settings.


#include "StreetSpline.h"
#include "Components/SplineComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"



//JUST IGNORE THIS WAS A DESPERATE ATTEMPT TO MAKE SOMETHING VERY CURSED
//is not really used anyway

// Sets default values
AStreetSpline::AStreetSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(m_DefaultRoot);

	m_Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	m_Spline->SetupAttachment(RootComponent);

	m_MeshComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("Mesh"));
	m_MeshComponent->SetupAttachment(RootComponent);
	m_MeshComponent1 = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("Mesh1"));
	m_MeshComponent1->SetupAttachment(RootComponent);
	m_MeshComponent2 = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("Mesh2"));
	m_MeshComponent2->SetupAttachment(RootComponent);

	
}

void AStreetSpline::UpdateVehicles(float DeltaTime)
{

	m_DeltaCounter += DeltaTime;

	for (int i = m_NumberOfInstances - 1; i >= 0; i--)
	{
		float desiredDist = (m_MaxLengthOfCar + m_DistBetweenVehicles) * i + m_DistToAdd * m_DeltaCounter;
		//UE_LOG(LogTemp, Display, TEXT("Spline:%f,Dist:%f"), m_Spline->GetSplineLength(), m_DistBetweenVehicles * i + m_DistToAdd * m_DeltaCounter);
		if ((m_MaxLengthOfCar + m_DistBetweenVehicles) * i + m_DistToAdd * m_DeltaCounter > m_Spline->GetSplineLength())
		{
			desiredDist -= m_Spline->GetSplineLength();
			//UE_LOG(LogTemp, Display, TEXT("Dist:%f,I=%d"), m_DistBetweenVehicles * i + m_DistToAdd * desiredDist,i);
		}

		FTransform trans = FTransform(m_Spline->GetWorldLocationAtDistanceAlongSpline(desiredDist));

		FRotator newRot = FRotator(m_Spline->GetWorldRotationAtDistanceAlongSpline(desiredDist));
		trans.SetRotation(newRot.Quaternion());
		m_MeshComponent->UpdateInstanceTransform(i, trans, true);

	}

	m_MeshComponent->MarkRenderStateDirty();
	if (m_DistToAdd * m_DeltaCounter > m_Spline->GetSplineLength())
	{
		m_DeltaCounter = 0;
	}
}

// Called when the game starts or when spawned
void AStreetSpline::BeginPlay()
{
	Super::BeginPlay();

	m_NumberOfInstances = static_cast<int>(m_Spline->GetSplineLength() / (m_MaxLengthOfCar + m_DistBetweenVehicles));

	UE_LOG(LogTemp, Display, TEXT("NumOfInstances:%d"), m_NumberOfInstances)

	UE_LOG(LogTemp, Display, TEXT("SplineLen:%f"), m_Spline->GetSplineLength())

	for (int i = 0; i < m_NumberOfInstances; ++i)
	{
		m_MeshComponent->AddInstance(FTransform(FVector(0, 0, 0)));
		m_MeshComponent->SetCustomDataValue(i, 0, FMath::FRandRange(0.0f, 1.0f));
		m_MeshComponent->SetCustomDataValue(i, 1, FMath::FRandRange(0.0f, 1.0f));
		m_MeshComponent->SetCustomDataValue(i, 2, FMath::FRandRange(0.0f, 1.0f));
	}

	UpdateVehicles(0);
	SetActorTickEnabled(false);
	
}

// Called every frame
void AStreetSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
