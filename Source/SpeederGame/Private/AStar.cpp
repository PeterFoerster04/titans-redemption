// Fill out your copyright notice in the Description page of Project Settings.


#include "AStar.h"


AStar::AStar(UNavMeshGrid3D* a_MeshGridAsset)
{
	m_MeshGridAsset = a_MeshGridAsset;
}

TArray<FVector> AStar::CalculatePath(FIntVector3 a_StartPos, FIntVector3 a_Destination)
{
	if (m_MeshGridAsset == nullptr||m_MeshGridAsset->m_Grid.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not generate path: Asset or grid is null."))
		return TArray<FVector>();
	}
	
	//creating start and end node and adding start node to the list of still to be visited nodes
	Node* Start = new Node(a_StartPos,m_MeshGridAsset->m_NavMeshResolution.Y, m_MeshGridAsset->m_NavMeshResolution.Z);
	Node* Destination = new Node(a_Destination, m_MeshGridAsset->m_NavMeshResolution.Y, m_MeshGridAsset->m_NavMeshResolution.Z);

	//if start or destination exceed the max grid resolution, the positions are outside the bounds, same for negative values
	if (Destination->GetGridPosX()> m_MeshGridAsset->m_NavMeshResolution.X||
		Destination->GetGridPosY() > m_MeshGridAsset->m_NavMeshResolution.Y||
		Destination->GetGridPosZ() > m_MeshGridAsset->m_NavMeshResolution.Z||
		Start->GetGridPosX() > m_MeshGridAsset->m_NavMeshResolution.X|| 
		Start->GetGridPosY() > m_MeshGridAsset->m_NavMeshResolution.Y|| 
		Start->GetGridPosZ() > m_MeshGridAsset->m_NavMeshResolution.Z||Destination->GetGridIndex()<0||Start->GetGridIndex()<0)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not generate path: Start or destination is out of bounds."))
		return TArray<FVector>();
	}
	//error functions for debugging, should not hit if nav mesh id correctly updated and set up
	if (m_MeshGridAsset->m_Grid[Destination->GetGridIndex()])
	{
		UE_LOG(LogTemp, Error, TEXT("Could not generate path: Destination is blocked."))
		UE_LOG(LogTemp, Error, TEXT("Failed Dest Pos:%d,%d,%d"), Destination->GetGridPosX(), Destination->GetGridPosY(), Destination->GetGridPosZ())
		DeleteRawPointer(Start);
		DeleteRawPointer(Destination);
		return TArray<FVector>();
	}
	if (m_MeshGridAsset->m_Grid[Start->GetGridIndex()])
	{
		UE_LOG(LogTemp, Error, TEXT("Could not generate path: Start is blocked."))
		UE_LOG(LogTemp, Error, TEXT("Failed Start Pos:%d,%d,%d"),Start->GetGridPosX(), Start->GetGridPosY(), Start->GetGridPosZ())
		DeleteRawPointer(Start);
		DeleteRawPointer(Destination);
		return TArray<FVector>();
	}


	
	//nodes to visit
	TArray<Node*> OpenList{};
	//visited nodes
	TArray<Node*> ClosedList{};
	//neighbours of the current node
	TArray<Node*> neighbours{};
	

	//used custom comparison func since c++ as well as unreal do not provide a priority queue that binds an element to an index like c#
	OpenList.HeapPush(Start, FCostComparison());
	UE_LOG(LogTemp,Display,TEXT("Now Calculating Path"))
	//exiting when there are no nodes more left to visit
	while(OpenList.Num() > 0&&!m_StopCalculation)
	{

		//getting the first node (the node ith the highest priority) from the "to be visited" list
		Node* OutNode = new Node{ FIntVector3(),0,0 };
		OpenList.HeapPop(OutNode,FCostComparison());
		//putting that node into the visited list and setting the current node to check 
		ClosedList.Push(OutNode);
		Node* CurrentNode = ClosedList.Last();

		//getting all 26 neighbours of the current node
		neighbours = GetNeighbors(CurrentNode);
		
		//exit if the current node is the destination
		//indices are compared since they are unique
		if (CurrentNode->GetGridIndex() == Destination->GetGridIndex())
		{
			//please change later return value to vectors instead of nodes
			
			m_PathViaNodes = PathRetrace(*Start, *CurrentNode);
			ClearList(&neighbours);
			ClearList(&OpenList);
			ClearList(&ClosedList);

			return ConvertToWorldCoordinates(m_PathViaNodes);
		}

		//GENERAL EXPLANATION:
		//checking all neighbours of the current node 
		//neighbour is skipped if they were already visited or if they are blocked
		//if they are already in the "still to be visited list" their cost gets updated
		//if not they are added to it and costs will be calculated

		for (Node* NeighborNode :neighbours)
		{
			//check if node is blocked by getting the index and checking in bool grid array, gets skipped if already in close list
			if (m_MeshGridAsset->m_Grid[NeighborNode->GetGridIndex()] || CheckArrayContains(NeighborNode,&ClosedList))
			{
				continue;
			}
			//only updating costs if node is already in open list
			float traceCostToNeighbor = CurrentNode->GetGCost() + GetDistanceBetweenNodes(*CurrentNode, *NeighborNode);
			//updating neighbours g cost if better path is found and calculating all costs for new nodes
			if (traceCostToNeighbor < NeighborNode->GetGCost() || !CheckArrayContains(NeighborNode,&OpenList))
			{
				
				NeighborNode->SetGCost(traceCostToNeighbor);
				NeighborNode->SetHCost(GetDistanceBetweenNodes(*NeighborNode, *Destination));
				NeighborNode->CalcFCost();
				//setting the parent to retrace the path later
				NeighborNode->SetParent(CurrentNode);
				
				if(!CheckArrayContains(NeighborNode, &OpenList))
				{
					OpenList.HeapPush(NeighborNode, FCostComparison());
				}
			}
		}
	
	}


	UE_LOG(LogTemp,Error,TEXT("Path Generation Failed"))
	return TArray<FVector>();
}

TArray<FVector> AStar::ConvertToWorldCoordinates(TArray<Node> a_GridPath)
{
	//this function takes the calculated path by Astar and turns it into useful information lol
	TArray<FVector> pathViaVectors{};
	FVector boundsMin = m_MeshGridAsset->m_NavMeshBounds.Min;
	FVector voxelSize = m_MeshGridAsset->m_VoxelSize;

	for (int i = 0; i < a_GridPath.Num(); ++i)
	{
		
		FVector vec{};

		vec.X = (a_GridPath[i].GetGridPosX()* voxelSize.X) + voxelSize.X / 2.0f;
		vec.Y = (a_GridPath[i].GetGridPosY()* voxelSize.Y) + voxelSize.Y / 2.0f;
		vec.Z = (a_GridPath[i].GetGridPosZ()* voxelSize.Z) + voxelSize.Z / 2.0f;

		vec += boundsMin;
		pathViaVectors.Add(vec);

	}

	return pathViaVectors;
}

TArray<Node*> AStar::GetNeighbors(Node* a_Node)
{
	FIntVector3 nodeCoords = a_Node->GetGridPos();
	TArray<Node*>Neighbors{};
	int GridResY = m_MeshGridAsset->m_NavMeshResolution.Y;
	int GridResZ = m_MeshGridAsset->m_NavMeshResolution.Z;
	int GridResX = m_MeshGridAsset->m_NavMeshResolution.X;
	
	//prevent pathfinding on edges of volume
	//probably need to be changed to something more specific later lets see
	if(nodeCoords.X == 0 || nodeCoords.Y == 0|| nodeCoords.Z == 0 || nodeCoords.X == GridResX-1 || nodeCoords.Y == GridResY-1 || nodeCoords.Z == GridResZ-1){
		return TArray<Node*>();
	}

	//Neighbors Visualized:
	//
	//		CLT
	//		FNW
	//		IQZ
	//		
	//	CFI	IQZ	ZWT
	//	BEH	HPY	YVS
	//	ADG	GOX	XUR
	//
	//		GOX
	//		DMU
	//		AJR
	//
	//		AJR
	//		BKS
	//		CLT

	//getting front neighbors
	
	for (int z = -1; z < 2; ++z)
	{
		//z=-1 && y=-1 -> G
		//z=1 && y=1 -> Z
		for (int y = -1; y < 2; ++y)
		{

			Node* NNode = new Node(FIntVector3(nodeCoords.X - 1, nodeCoords.Y + y, nodeCoords.Z + z), GridResY, GridResZ);
			Neighbors.Add(NNode);
		}
	}
	
	//getting back neighbors
	
	for (int z = -1; z < 2; ++z)
	{
		//z=-1 && y=-1 -> A
		//z=1 && y=1 -> T
		for (int y = -1; y < 2; ++y)
		{
			Node* NNode = new Node(FIntVector3(nodeCoords.X + 1, nodeCoords.Y + y, nodeCoords.Z + z), GridResY, GridResZ);
			Neighbors.Add(NNode);
		}
	}

	//getting middle neighbors
	//F E D  M  U V W N
	for (int z = -1; z < 2; ++z)
	{
	
		for (int y = -1; y < 2; ++y){

			//skip the middle since it is the node itself
			if (y == 0 && z == 0) {
				continue;
			}
			Node* NNode = new Node(FIntVector3(nodeCoords.X, nodeCoords.Y + y, nodeCoords.Z + z), GridResY, GridResZ);
			Neighbors.Add(NNode);
		}
	}

	return Neighbors;
}

float AStar::GetDistanceBetweenNodes(Node a_N1, Node a_N2)
{

	float distX = FMath::Abs(a_N1.GetGridPosX()-a_N2.GetGridPosX());
	float distY = FMath::Abs(a_N1.GetGridPosY() - a_N2.GetGridPosY());
	float distZ = FMath::Abs(a_N1.GetGridPosZ() - a_N2.GetGridPosZ());
	
	float min = FMath::Min3(distX,distY,distZ);
	float max = FMath::Max3(distX, distY, distZ);
	
	float tripleAxis = min;
	float doubleAxis = distX + distY + distZ - max - 2 * min;
	float singleAxis = max - doubleAxis - tripleAxis;


	return 10.0f * singleAxis + 14.0f * doubleAxis + 17.0f * tripleAxis;
}

TArray<Node> AStar::PathRetrace(Node a_Start, Node a_Destination)
{

	TArray<Node> path{};
	Node currNode = a_Destination;
	//exit if start node is reached, since it has no parent
	while(currNode.GetParent() != nullptr)
	{
		path.Add(currNode);
		currNode = *(currNode.GetParent());
		
		

	}
	Algo::Reverse(path);


	return path;
}

bool AStar::CheckArrayContains(Node* a_Node, TArray<Node*>* a_Array)
{
	for (Node* node : *a_Array)
	{
		if(node->GetGridIndex() == a_Node->GetGridIndex())
		{
			return true;
		}

	}

	return false;
}

void AStar::ClearList(TArray<Node*>* a_Array)
{
	for (auto pointer : *a_Array)
	{
		if(pointer != nullptr)
		{
			DeleteRawPointer(pointer);
		}
		
	}

}





