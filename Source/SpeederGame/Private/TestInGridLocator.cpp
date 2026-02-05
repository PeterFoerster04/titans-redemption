// Fill out your copyright notice in the Description page of Project Settings.


#include "TestInGridLocator.h"

//this objects converts positions between world and nav mesh grid for debugging

// Sets default values
ATestInGridLocator::ATestInGridLocator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(m_DefaultRoot);
}

// Called when the game starts or when spawned
void ATestInGridLocator::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATestInGridLocator::ShowPositionInGrid()
{
	if(m_MeshGridAsset->m_VoxelSize.X == 0 || m_MeshGridAsset->m_VoxelSize.Y == 0|| m_MeshGridAsset->m_VoxelSize.Z == 0)
	{
		UE_LOG(LogTemp, Type::Error, TEXT("Grid is null"));
		return;
	}

	FVector posInW = this->GetActorLocation();

	UE_LOG(LogActor, Type::Error, TEXT("World Pos: %f, %f, %f"), posInW.X,posInW.Y , posInW.Z);
	
	FVector gridvec = posInW - m_MeshGridAsset->m_NavMeshBounds.Min;


	//this dumb floating point almost caused my death at 5am in the morning, just deleting it by a c style cast suits perfectly 
	gridvec.X = (int)(gridvec.X / m_MeshGridAsset->m_VoxelSize.X);
	gridvec.Y = (int)(gridvec.Y / m_MeshGridAsset->m_VoxelSize.Y);
	gridvec.Z = (int)(gridvec.Z / m_MeshGridAsset->m_VoxelSize.Z);
	
	UE_LOG(LogTemp, Display, TEXT("VoxelSizeXYZ: %f, %f, %f"), m_MeshGridAsset->m_VoxelSize.X, m_MeshGridAsset->m_VoxelSize.Y, m_MeshGridAsset->m_VoxelSize.Z);
	UE_LOG(LogTemp, Display, TEXT("Resolution: %f, %f, %f"), m_MeshGridAsset->m_NavMeshResolution.X, m_MeshGridAsset->m_NavMeshResolution.Y, m_MeshGridAsset->m_NavMeshResolution.Z);
	UE_LOG(LogTemp, Display, TEXT("Position in Grid: %f, %f, %f"), gridvec.X, gridvec.Y, gridvec.Z);

	//how to convert from coords to index
	int index = gridvec.X * m_MeshGridAsset->m_NavMeshResolution.Y * m_MeshGridAsset->m_NavMeshResolution.Z + gridvec.Y * m_MeshGridAsset->m_NavMeshResolution.Z + gridvec.Z;

	int a_ResY = m_MeshGridAsset->m_NavMeshResolution.Y;
	int a_ResZ = m_MeshGridAsset->m_NavMeshResolution.Z;

	//How to convert back to Coords from index
	int PosX = (int)(index / (a_ResY * a_ResZ));
	int PosY = (int)((index - PosX * a_ResY * a_ResZ) / a_ResZ);
	int PosZ = (int)(index - a_ResZ * (PosY + a_ResY * PosX));



	UE_LOG(LogTemp, Display, TEXT("Grid Coordinates:%d, %d, %d"), PosX,PosY,PosZ);
	UE_LOG(LogTemp,Display, TEXT("Grid Index:%d"), index);


}

// Called every frame
void ATestInGridLocator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

