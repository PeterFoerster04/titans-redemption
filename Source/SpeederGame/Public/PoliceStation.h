// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CaseFile.h"
#include "CaseMission.h"
#include "PlayerStats.h"
#include "SpeederChaseGameInstance.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PoliceStation.generated.h"


class ACriminalSpawnPointManager;
class AAircraft;
class ACriminal;
class UCameraComponent;
UCLASS()
class SPEEDERGAME_API APoliceStation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoliceStation();

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	USceneComponent* m_DefaultRoot = nullptr;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UStaticMeshComponent* m_Mesh = nullptr;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UBoxComponent* m_TriggerBox;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UCameraComponent* m_GarageCam;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UCameraComponent* m_FileSelectionCam;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	USceneComponent* m_PlayerGaragePos;
	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	USceneComponent* m_PlayerOutSpawnPos;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	USceneComponent* m_File1SpawnPoint;
	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	USceneComponent* m_File2SpawnPoint;
	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	USceneComponent* m_File3SpawnPoint;

	UPROPERTY(EditAnywhere, Category = Spawn, meta = (AllowPrivateAccess))
	ACriminalSpawnPointManager* m_CriminalSpawnManager;


	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = Comps, meta = (AllowPrivateAccess))
	class UPoliceStationUIManager* m_PoliceUIManager;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	USpeederChaseGameInstance* m_GameInstance;




	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//Files/Missions
	
	//to spawn the actual blueprint with the correct widget component instead of a static class
	UPROPERTY(EditAnywhere, Category = Files, meta = (AllowPrivateAccess))
	TSubclassOf<ACaseFile> m_BPFile;

	UPROPERTY(BlueprintReadOnly, Category = Files, meta = (AllowPrivateAccess))
	ACaseFile* m_CaseFile1 = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = Files, meta = (AllowPrivateAccess))
	ACaseFile* m_CaseFile2 = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = Files, meta = (AllowPrivateAccess))
	ACaseFile* m_CaseFile3 = nullptr;

	UPROPERTY()
	UWorld* m_World;

	UPROPERTY(BlueprintReadWrite)
	ACaseFile* m_SelectedFile;

	UPROPERTY()
	UPlayerStats* m_PlayerStats;

	UPROPERTY()
	AAircraft* m_PlayerVehicle;

	UPROPERTY()
	APlayerController* m_PlayerController;

	//Current mission
	UPROPERTY(BlueprintReadOnly)
	ACaseMission* m_CurrentMission = nullptr;
	bool m_CurrentMissionAbsolved = false;
	bool m_PlayerWantsNewMissions = true;
	bool m_PlayerHasChosenFile = false;

	UPROPERTY(BlueprintReadOnly)
	float m_TimeBonusReward = 0.0f;

	//since mission gets deleted after it was accomplished, saving some old values of it for display
	UPROPERTY(BlueprintReadOnly)
	float m_MissionAbsolvedTime = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	float m_MissionInitialTime = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	float m_BaseRewardForAbsolvedMission = 0.0f;



	UPROPERTY(EditAnywhere, Category = Missions, meta = (AllowPrivateAccess))
	float m_MaxMissionTimeLimit = 600.0f;
	UPROPERTY(EditAnywhere, Category = Missions, meta = (AllowPrivateAccess))
	float m_MinMissionTimeLimit = 300.0f;

	UPROPERTY(BlueprintReadOnly)
	bool m_PLayerIsInGarage = false;
	bool m_ParkingPlayerIn = false;

	UPROPERTY(EditAnywhere, Category = Criminals, meta = (AllowPrivateAccess))
	TSubclassOf<ACriminal> m_Criminal1BP;

	UPROPERTY()
	ACriminal* m_CurrentCriminal;

	FVector m_CriminalSpawnPos;

	float m_RandomSpeedMultiplier = 0.0f;
	float m_MinSpeedMultiplier = 4000.0f;
	float m_MaxSpeedMultiplier = 6700.0f;
	float m_MinReward = 500.0f;
	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void CreateNewFiles();
	UFUNCTION(BlueprintCallable)
	void DeleteOldFiles();
	void CreateAndConfigureFile(ACaseFile** a_File, USceneComponent* a_SpawnPos);

	int PickRandomSpawnPos();
	FVector ConvertFromIndexToPosition(int a_Index);
	void CreateMissionBasedOnFile();
	float CalculateMissionReward();
	float CalculateSpeedMultiplier();
	float PredictMaxSpeedInKmph();
	void CalculateBonusReward();
	void DeInitMission();

	void SpawnCriminal();
	void ConfigureCriminalValues();

	bool m_TimerCounts = false;
	bool m_StartFade = false;
	float m_TimerDuration = 2.0f;
	float m_CurrTime;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void KickPlayerOutOfGarage();

	UFUNCTION(BlueprintCallable)
	void InitiateSelectedMission();

	UFUNCTION(BlueprintCallable)
	void SwapPoliceCams();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowWinScreen();
	UFUNCTION(BlueprintImplementableEvent)
	void ShowFailScreen(bool a_ReasonWasTime);

	

};
