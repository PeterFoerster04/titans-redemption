// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavMeshGrid3D.h"
#include "Node.h"


/**
 * 
 */
class FCostComparison;


class AStar 
{
	
	UNavMeshGrid3D* m_MeshGridAsset = nullptr;

public:
	AStar(UNavMeshGrid3D* a_MeshGridAsset);
	

	TArray<FVector> CalculatePath(FIntVector3 a_StartPos,FIntVector3 a_Destination);
	//the pathfinder calculates in grid positions
	//th AI however needs specific world coordinates represented by an array of vectors
	TArray<FVector> ConvertToWorldCoordinates(TArray<Node> a_GridPath);

	TArray<Node*> GetNeighbors(Node* a_Node);

	float GetDistanceBetweenNodes(Node a_N1, Node a_N2);
	TArray<Node> PathRetrace(Node a_Start, Node a_Destination);

	//used for debugging
	TArray<Node> m_PathViaNodes{};
	bool m_StopCalculation = false;

private:
	bool CheckArrayContains(Node* a_Node, TArray<Node*>* a_Array);
	void ClearList(TArray<Node*>* a_Array);
};


//custom comparison function to compare nodes for priority queue :)
class FCostComparison
{
public:
	bool operator()(Node n1,Node n2)const{
		//the f cost is compared first, if they are equal, the heuristic cost is compared
		if(n1.GetFCost()<n2.GetFCost())
		{
			return true;
		}
		if(n1.GetFCost()==n2.GetFCost())
		{
			if(n1.GetHCost()<n2.GetHCost())
			{
				return true;
			}
			return false;
		}
		return false;
		
	}
};

#define DeleteRawPointer(pointer) delete pointer; pointer = nullptr

