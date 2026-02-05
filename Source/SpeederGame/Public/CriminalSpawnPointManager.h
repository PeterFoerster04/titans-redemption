// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CriminalSpawnPointManager.generated.h"

UCLASS()
class SPEEDERGAME_API ACriminalSpawnPointManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACriminalSpawnPointManager();


	UPROPERTY()
	TArray<FVector> m_SpawnPositions;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	USceneComponent* m_DefaultRoot = nullptr;

	UPROPERTY(EditAnywhere, Category = Spawns, meta = (AllowPrivateAccess))
	USceneComponent* m_Spawn0;
	UPROPERTY(EditAnywhere, Category = Spawns, meta = (AllowPrivateAccess))
	USceneComponent* m_Spawn1;
	UPROPERTY(EditAnywhere, Category = Spawns, meta = (AllowPrivateAccess))
	USceneComponent* m_Spawn2;
	UPROPERTY(EditAnywhere, Category = Spawns, meta = (AllowPrivateAccess))
	USceneComponent* m_Spawn3;
	UPROPERTY(EditAnywhere, Category = Spawns, meta = (AllowPrivateAccess))
	USceneComponent* m_Spawn4;
	UPROPERTY(EditAnywhere, Category = Spawns, meta = (AllowPrivateAccess))
	USceneComponent* m_Spawn5;
	UPROPERTY(EditAnywhere, Category = Spawns, meta = (AllowPrivateAccess))
	USceneComponent* m_Spawn6;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(CallInEditor)
	void ShowSpawnPositions();

	void InitList();

};
