// Fill out your copyright notice in the Description page of Project Settings.


#include "Node.h"

Node::Node(FIntVector3 a_GridPos, int a_GridResY, int a_GridResZ)
{

	m_GridPos = a_GridPos;
	m_GridIndex = m_GridPos.X * a_GridResY * a_GridResZ + m_GridPos.Y * a_GridResZ + m_GridPos.Z;

}


Node::~Node()
{
}
