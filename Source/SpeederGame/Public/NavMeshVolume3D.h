// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavMeshGrid3D.h"
#include "GameFramework/Volume.h"
#include "PathFindingThread.h"
#include "AStar.h"
#include "NavMeshVolume3D.generated.h"


/**
 * 
 */
UCLASS()
class SPEEDERGAME_API ANavMeshVolume3D : public AVolume
{
	GENERATED_BODY()
public:
	ANavMeshVolume3D();

protected:
	UPROPERTY(EditAnywhere,Category= Generation,meta=(AllowPrivateAccess))
	uint16 m_ResolutionX = 0;
	UPROPERTY(EditAnywhere, Category = Generation, meta = (AllowPrivateAccess))
	uint16 m_ResolutionY = 0;
	UPROPERTY(EditAnywhere, Category = Generation, meta = (AllowPrivateAccess))
	uint16 m_ResolutionZ = 0;
	UPROPERTY(EditAnywhere, Category = Generation, meta = (AllowPrivateAccess))
	int m_DebugBoxIndex = 0;
	UPROPERTY(EditAnywhere, Category = Generation, meta = (AllowPrivateAccess))
	bool m_OnlyDrawIndexedBox = false;

	UPROPERTY(EditAnywhere, Category = Generation, meta = (AllowPrivateAccess))
	USceneComponent* m_StartTransform;
	UPROPERTY(EditAnywhere, Category = Generation, meta = (AllowPrivateAccess))
	USceneComponent* m_EndTransform;

	UPROPERTY(EditAnywhere, Category = Generation, meta = (AllowPrivateAccess))
	UNavMeshGrid3D* m_MeshGridAsset;

	AStar* m_Pathfinder = nullptr;
	PathFindingThread* m_Thread = nullptr;


	UFUNCTION(CallInEditor, Category = Generation, meta = (AllowPrivateAccess))
	void Generate3DNavMesh();

	UFUNCTION(CallInEditor, Category = Generation, meta = (AllowPrivateAccess))
	void Preview3DNavMesh();

	UFUNCTION(CallInEditor, Category = Generation, meta = (AllowPrivateAccess))
	void ClearPreview();

	UFUNCTION(CallInEditor, Category = PathFinding, meta = (AllowPrivateAccess))
	void ShowCalculatedPath();

	UFUNCTION(CallInEditor, Category = PathFinding, meta = (AllowPrivateAccess))
	void TestPathFinding();

	UFUNCTION(CallInEditor, Category = PathFinding, meta = (AllowPrivateAccess))
	void TestPathFindingMultiThreaded();
	

	UFUNCTION(CallInEditor, Category = PathFinding, meta = (AllowPrivateAccess))
	void ShowNodeNeigbors();

	UPROPERTY(EditAnywhere, Category = PathFinding, meta = (AllowPrivateAccess))
	bool m_DisplayPath = false;


	bool CheckValidity();

	

	void DisplayVectorPath(TArray<FVector>& a_Array);

	FIntVector3 ConvertFromWorldPosToGridPos(FVector a_WPos);
	
};
