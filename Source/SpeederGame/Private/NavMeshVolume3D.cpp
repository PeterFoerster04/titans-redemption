// Fill out your copyright notice in the Description page of Project Settings.


#include "NavMeshVolume3D.h"

#include "Components/BrushComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tasks/Task.h"

ANavMeshVolume3D::ANavMeshVolume3D()
{
	m_StartTransform = CreateDefaultSubobject<USceneComponent>("StartPos");
	m_StartTransform->SetupAttachment(RootComponent);

	m_EndTransform = CreateDefaultSubobject<USceneComponent>("EndPos");
	m_EndTransform->SetupAttachment(RootComponent);
}

void ANavMeshVolume3D::Generate3DNavMesh()
{
	//only generate if conditions are met
	if (!CheckValidity()) return;

	//getting the box the size of the volume
	FBox bounds = this->GetBounds().GetBox();
	FVector boundExtents = bounds.GetSize();
	UE_LOG(LogTemp, Type::Display, TEXT("Generated Mesh, Volume Bounds Min: %f, %f, %f"),bounds.Min.X, bounds.Min.Y, bounds.Min.Z);

	//getting th size of the volume and dividing it by the set resolution to get the size of voxels in the created grid

	float voxelSizeX = boundExtents.X/ m_ResolutionX;
	float voxelSizeY = boundExtents.Y/ m_ResolutionY;
	float voxelSizeZ = boundExtents.Z/ m_ResolutionZ;

	m_MeshGridAsset->m_VoxelSize = FVector(voxelSizeX, voxelSizeY, voxelSizeZ);

	//clearing the current grid array and reserving new space 
	m_MeshGridAsset->m_Grid.Empty(m_ResolutionY * m_ResolutionY * m_ResolutionZ);
	//saving the size and resolution into the asset
	m_MeshGridAsset->m_NavMeshResolution = FVector(m_ResolutionY,m_ResolutionY,m_ResolutionZ);
	m_MeshGridAsset->m_NavMeshSize = boundExtents;
	m_MeshGridAsset->m_NavMeshBounds = bounds;

	m_MeshGridAsset->m_Location = this->GetActorLocation();


	//iterating through all voxels and determine if the voxel intersects with a mesh
	
	for (uint16 x = 0; x < m_ResolutionX; ++x)
	{
		for (uint16 y = 0; y < m_ResolutionY; ++y)
		{
			for (uint16 z = 0; z < m_ResolutionZ; ++z)
			{

				//creating an arbitrary box and tracing with the bounds of the box if voxel intersects with a mesh, a bool is inserted into the grid array
				//box takes 2 vectors, start and end point to create a box
				//xyz is the current box that is created to index through the volume
				//xyz size: the length of the box sides
				//the bounds of the volume (position) needs to be factored in to the vectors, otherwise the boxes will not be created at the volume in world space
				FBox voxel(FVector(x * voxelSizeX + bounds.Min.X, y * voxelSizeY + bounds.Min.Y, z * voxelSizeZ + bounds.Min.Z), FVector(x * voxelSizeX + voxelSizeX + bounds.Min.X, y * voxelSizeY + voxelSizeY + bounds.Min.Y, z * voxelSizeZ + voxelSizeZ + bounds.Min.Z));
				FHitResult hitResult;
				bool hit =UKismetSystemLibrary::BoxTraceSingle(
					this, voxel.GetCenter(),
					voxel.GetCenter(), voxel.GetExtent(),
					FRotator::ZeroRotator, TraceTypeQuery_MAX,
					true, TArray<AActor*>(),
					EDrawDebugTrace::Type::None, hitResult, true);
				m_MeshGridAsset->m_Grid.Push(hit);
				
			}
		}
	}
	m_MeshGridAsset->GetPackage()->MarkPackageDirty();
	
}


//to visualize the current grid (very laggy)
void ANavMeshVolume3D::Preview3DNavMesh()
{

	if (!CheckValidity()) return;


	FBox bounds = this->GetBounds().GetBox();
	FVector boundExtents = bounds.GetSize();

	float voxelSizeX = boundExtents.X / m_ResolutionX;
	float voxelSizeY = boundExtents.Y / m_ResolutionY;
	float voxelSizeZ = boundExtents.Z / m_ResolutionZ;
	 
	//changing th resolution without regenerating causes a wrong grid
	if(m_MeshGridAsset->m_NavMeshResolution != FVector(m_ResolutionX,m_ResolutionY,m_ResolutionZ)||m_MeshGridAsset->m_NavMeshSize !=boundExtents)
	{
		UE_LOG(LogActor, Error, TEXT("Asset out of date: Regenrate asset"));
		return;
	}

	for (uint16 x = 0; x < m_ResolutionX; ++x)
	{
		for (uint16 y = 0; y < m_ResolutionY; ++y)
		{
			for (uint16 z = 0; z < m_ResolutionZ; ++z)
			{
				FBox voxel(FVector(x * voxelSizeX + bounds.Min.X, y * voxelSizeY + bounds.Min.Y, z * voxelSizeZ + bounds.Min.Z), FVector(x * voxelSizeX + voxelSizeX + bounds.Min.X, y * voxelSizeY + voxelSizeY + bounds.Min.Y, z * voxelSizeZ + voxelSizeZ + bounds.Min.Z));
				FColor boxColor(230,230,230);
				//iterating through bool array to check if current voxel has intersected with something
				
				if (m_OnlyDrawIndexedBox && (x * m_ResolutionY * m_ResolutionZ + y * m_ResolutionZ + z) == m_DebugBoxIndex)
				{
					//marking indexed voxel red
					boxColor = FColor::Red;
					DrawDebugBox(GetWorld(), voxel.GetCenter(), voxel.GetExtent(), boxColor, false, 600);
				}
				else if (!m_OnlyDrawIndexedBox && m_MeshGridAsset->m_Grid.Num() == m_ResolutionY * m_ResolutionY * m_ResolutionZ && m_MeshGridAsset->m_Grid[x * m_ResolutionY * m_ResolutionZ + y * m_ResolutionZ + z])
				{
					//marking intersecting voxels blue
					boxColor = FColor::Blue;
					DrawDebugBox(GetWorld(), voxel.GetCenter(), voxel.GetExtent(), boxColor, false, 600);
				}
				
				
			}
		}
	}

}
//the next functions are mostly for debugging and displaying the path and nav mesh volume via debug lines and boxes
void ANavMeshVolume3D::ClearPreview()
{

	FlushPersistentDebugLines(this->GetWorld());
}

void ANavMeshVolume3D::ShowCalculatedPath()
{
	if(m_Thread == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Error: Thread is not generated."));
		return;
	}


	if (m_DisplayPath && !m_Thread->m_FinishedPath.IsEmpty())
	{
		m_OnlyDrawIndexedBox = true;

		for (Node node : m_Pathfinder->m_PathViaNodes)
		{
			m_DebugBoxIndex = node.GetGridIndex();
			Preview3DNavMesh();
		}
		m_OnlyDrawIndexedBox = false;
		DisplayVectorPath(m_Thread->m_FinishedPath);
	}
}

void ANavMeshVolume3D::TestPathFinding()
{


	if(m_MeshGridAsset == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Mesh Grid Asset Is Null LuL"))
		return;
	}

	FIntVector3 startpoint = ConvertFromWorldPosToGridPos(m_StartTransform->GetComponentLocation());
	FIntVector3 endpoint = ConvertFromWorldPosToGridPos(m_EndTransform->GetComponentLocation());

	UE_LOG(LogTemp, Display, TEXT("Start:%d, %d, %d"), startpoint.X, startpoint.Y, startpoint.Z);
	UE_LOG(LogTemp, Display, TEXT("Destination:%d, %d, %d"), endpoint.X, endpoint.Y, endpoint.Z);
	ClearPreview();
	AStar astar = AStar(m_MeshGridAsset);

	TArray<FVector> path = astar.CalculatePath(startpoint,endpoint);



}

void ANavMeshVolume3D::TestPathFindingMultiThreaded()
{
	if (m_MeshGridAsset == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Mesh Grid Asset Is Null LuL"))
			return;
	}

	FIntVector3 startpoint = ConvertFromWorldPosToGridPos(m_StartTransform->GetComponentLocation());
	FIntVector3 endpoint = ConvertFromWorldPosToGridPos(m_EndTransform->GetComponentLocation());

	UE_LOG(LogTemp, Display, TEXT("Start:%d, %d, %d"), startpoint.X, startpoint.Y, startpoint.Z);
	UE_LOG(LogTemp, Display, TEXT("Destination:%d, %d, %d"), endpoint.X, endpoint.Y, endpoint.Z);
	ClearPreview();

	m_Pathfinder = new AStar(m_MeshGridAsset);
	m_Thread = new PathFindingThread( m_Pathfinder,startpoint,endpoint);

	
	if (m_DisplayPath && !m_Thread->m_FinishedPath.IsEmpty())
	{
		m_OnlyDrawIndexedBox = true;

		for (Node node : m_Pathfinder->m_PathViaNodes)
		{
			m_DebugBoxIndex = node.GetGridIndex();
			Preview3DNavMesh();
		}
		m_OnlyDrawIndexedBox = false;
		DisplayVectorPath(m_Thread->m_FinishedPath);
	}

}


void ANavMeshVolume3D::ShowNodeNeigbors()
{
	//was only used for debugging
	int a_ResY = m_MeshGridAsset->m_NavMeshResolution.Y;
	int a_ResZ = m_MeshGridAsset->m_NavMeshResolution.Z;

	//How to convert back to Coords from index
	int PosX = (int)(m_DebugBoxIndex / (a_ResY * a_ResZ));
	int PosY = (int)((m_DebugBoxIndex - PosX * a_ResY * a_ResZ) / a_ResZ);
	int PosZ = (int)(m_DebugBoxIndex - a_ResZ * (PosY + a_ResY * PosX));

	ClearPreview();
	Node a_Node = Node(FIntVector(PosX,PosY,PosZ), m_MeshGridAsset->m_NavMeshResolution.Y, m_MeshGridAsset->m_NavMeshResolution.Z);

	AStar astar = AStar(m_MeshGridAsset);

	//TArray<Node> neighbors = astar.GetNeighbors(a_Node);

	//m_OnlyDrawIndexedBox = true;

	//for (Node Neighbor : neighbors)
	//{
	//	m_DebugBoxIndex = Neighbor.GetGridIndex();
	//	Preview3DNavMesh();
	//}
	//m_OnlyDrawIndexedBox = false;

}

bool ANavMeshVolume3D::CheckValidity()
{
	//rotating the volume results in wrong calculation of the grid, since the world position is taken into account 
	if (this->GetActorRotation() != FRotator::ZeroRotator)
	{
		UE_LOG(LogActor, Error, TEXT("Dude do not rotate the volume xD"));
		return false;
	}
	//resolutions have to be set to generate grid
	if (!m_ResolutionX || !m_ResolutionY || !m_ResolutionZ)
	{
		UE_LOG(LogActor, Error, TEXT("Dude please set resolution values..."));
		return false;
	}
	return true;
}

void ANavMeshVolume3D::DisplayVectorPath(TArray<FVector>& a_Array)
{
	FVector previous = a_Array[0];

	for (int i = 1; i < a_Array.Num(); ++i)
	{
		DrawDebugLine(GetWorld(), previous, a_Array[i], FColor::Green, false, 600.0f);
		previous = a_Array[i];
	}
}

FIntVector3 ANavMeshVolume3D::ConvertFromWorldPosToGridPos(FVector a_WPos)
{
	//checking if resolution is set, preventing division by 0
	if (m_MeshGridAsset->m_NavMeshResolution != FVector{0,0,0})
	{
		FVector gridvec = a_WPos - m_MeshGridAsset->m_NavMeshBounds.Min;
		//this dumb floating point almost caused my death at 5am in the morning, just deleting it by a c style cast suits perfectly 
		gridvec.X = (int)(gridvec.X / m_MeshGridAsset->m_VoxelSize.X);
		gridvec.Y = (int)(gridvec.Y / m_MeshGridAsset->m_VoxelSize.Y);
		gridvec.Z = (int)(gridvec.Z / m_MeshGridAsset->m_VoxelSize.Z);

		return FIntVector3(gridvec.X, gridvec.Y, gridvec.Z);
	}

	
	return FIntVector3(0,0,0);
	
}
