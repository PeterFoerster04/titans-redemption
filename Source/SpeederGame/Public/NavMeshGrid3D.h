// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NavMeshGrid3D.generated.h"

/**
 * 
 */
UCLASS()
class SPEEDERGAME_API UNavMeshGrid3D : public UPrimaryDataAsset
{
	GENERATED_BODY()


public:

	UPROPERTY()
	TArray<bool> m_Grid{};
	UPROPERTY(VisibleAnywhere)
	FVector m_NavMeshResolution{0,0,0};
	UPROPERTY(VisibleAnywhere)
	FVector m_NavMeshSize{0,0,0};
	UPROPERTY()
	FBox m_NavMeshBounds{};
	UPROPERTY(VisibleAnywhere)
	FVector m_Location{0,0,0};
	UPROPERTY(VisibleAnywhere)
	FVector m_VoxelSize{0,0,0};
	
};
