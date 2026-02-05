// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class Node
{
private:
	//the index is used to see if the node is blocked or not

	int m_GridIndex = 0;

	//th f cost actually does not have to be a member, is only used for debugging
	float m_GCost = 0;
	float m_HCost = 0;
	float m_FCost = 0;
	//parent is used to trace back the finished path
	Node* m_Parent = nullptr;

	FIntVector3 m_GridPos{};
	
public:
	Node(FIntVector3 a_GridPos, int a_GridResY, int a_GridResZ);
	
	~Node();

	//overloaded to compare indices as they are unique to each node in the grid
	//same index = same position in grid -> theoretically same node
	bool operator ==(Node other) const
	{
		if(m_GridIndex == other.GetGridIndex())
		{
			return true;
		}
		return false;
	}
	//getter and setter for every cost, self explanatory 
	inline FIntVector3 GetGridPos() { return m_GridPos; }
	inline int GetGridPosX() { return m_GridPos.X; }
	inline int GetGridPosY() { return m_GridPos.Y; }
	inline int GetGridPosZ() { return m_GridPos.Z; }
	
	inline void SetGCost(float a_Cost) { m_GCost = a_Cost; }
	inline void SetHCost(float a_Cost) { m_HCost = a_Cost; }
	inline void CalcFCost() { m_FCost = m_GCost + m_HCost; }

	inline float GetGCost() { return m_GCost; }
	inline float GetHCost() { return m_HCost; }
	inline float GetFCost() { return m_FCost; }

	inline Node* GetParent() { return m_Parent; }
	inline void SetParent(Node* a_Node) { m_Parent = a_Node; }
	inline int GetGridIndex() { return m_GridIndex; }

	
};
