// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavMeshGrid3D.h"
#include "TestInGridLocator.generated.h"

UCLASS()
class SPEEDERGAME_API ATestInGridLocator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestInGridLocator();

protected:


	UPROPERTY(EditAnywhere, Category = Generation, meta = (AllowPrivateAccess))
	UNavMeshGrid3D* m_MeshGridAsset;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	USceneComponent* m_DefaultRoot = nullptr;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION(CallInEditor, Category = Generation, meta = (AllowPrivateAccess))
	void ShowPositionInGrid();

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
