// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AStar.h"
#include "PathFindingThread.h"
#include "Criminal.h"
#include "CriminalController.generated.h"

/**
 * 
 */
UCLASS()
class SPEEDERGAME_API ACriminalController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

	virtual ~ACriminalController()override;

	UFUNCTION(BlueprintCallable, Category = "Pathfinding")
	void CalculateNewPath();
	UFUNCTION(BlueprintCallable, Category = "Pathfinding")
	void SetNextPositionOfPath();
	UFUNCTION(BlueprintCallable, Category = "Pathfinding")
	void FlyToNextPosition(float a_DeltaTime);


	UPROPERTY(EditAnywhere, Category = Controls, meta = (AllowPrivateAccess))
	bool m_Move = true;

	UPROPERTY(EditAnywhere, Category = Generation, meta = (AllowPrivateAccess))
	UNavMeshGrid3D* m_MeshGridAsset = nullptr;

	UPROPERTY()
	ACriminal* m_Criminal;
private:

	FVector m_NextVectorElement;

	FIntVector3 ConvertFromWorldPosToGridPos(FVector a_WPos);
	//gets called in calculate new path to get random destination
	FIntVector3 CalculateRandomPosition(FVector a_CurrentPos);

	FIntVector3 m_MaxRandomRange = { 45000,45000,3000 };
	FIntVector3 m_MinRandomRange = { 5000,5000,500 };
	bool m_UseMultithreading = true;

	TArray<FVector> m_PathNonMultiThreaded{};

	//to iterate through path array
	int m_CurrentVectorIndex = 0;


	bool m_NewPathRequired = true;
	bool m_ArrivedAtNextPoint = true;
	//since calculating is performed multi threaded, check if task ist still running
	bool m_StillCalculatingPath = false;

	TArray<FVector> m_CalculatedPath;
	AStar* m_Pathfinder = nullptr;

	PathFindingThread* m_Thread = nullptr;


	//bias variables
	//currently the pathfinding is very random, resulting in very weird changes of elevation
	//to smooth this a bit out, biases have to be applied, for example to have the vehicle not fly to high up all the time

	//if a destination is set above this limit, there will be a chance:
	// 1. that this destination will be chosen (small chance)
	// 2. that some height will be subtracted from the destination (middle chance)
	// 3. that the capped value will be chosen as the height (middle chance)
	// 4. that the current altitude remains (high chance)
	float m_AltitudeBiasCap = 40000.0f;
	float m_AltitudeMinSubractionValue = 3000.0f;
	float m_AltitudeMaxSubractionValue = 8000.0f;

	//RandomValue will be chosen between 0 and 100
	//if the random value is between one variable value and the value of the smaller variable, the one variable is chosen
	int m_KeepNewAltitudeChance = 10; //10% chance (0-9)
	int m_SubtractHeightChance = 30; // 20% chance (10-29)
	int m_TakeCappedAltitudeChance = 50; // 20% chance (30-49)
	int m_KeepOldAltitudeChance = 100; // 50% chance (50-100)
	
	int CalculateAltitudeWithBias(float a_AltitudeToEvaluate);
	
};
