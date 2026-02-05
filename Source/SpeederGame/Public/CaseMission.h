// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CaseFile.h"
#include "GameFramework/Actor.h"
#include "CaseMission.generated.h"

UCLASS()
class SPEEDERGAME_API ACaseMission : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACaseMission();

	UPROPERTY(BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess))
	ACaseFile* m_File = nullptr;

	UPROPERTY(BlueprintReadOnly)
	float m_TimeLimit = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = Comps, meta = (AllowPrivateAccess))
	class ACriminal* m_CriminalToChase = nullptr;


	UPROPERTY(BlueprintReadOnly)
	float m_CurrentTime = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	bool m_MissionWasAbsolved = false;
	UPROPERTY(BlueprintReadOnly)
	bool m_WasActivated = false;
	UPROPERTY(BlueprintReadOnly)
	bool m_MissionFailed = false;
	UPROPERTY(BlueprintReadOnly)
	float m_MissionReward = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void InitMission(ACaseFile* a_File, float a_TimeLimit, ACriminal* a_Criminal);
	void TickTimer(float a_DeltaTime);
	


};
