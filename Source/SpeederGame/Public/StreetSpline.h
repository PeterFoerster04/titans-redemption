// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StreetSpline.generated.h"


class UHierarchicalInstancedStaticMeshComponent;
UCLASS()
class SPEEDERGAME_API AStreetSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStreetSpline();


	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	USceneComponent* m_DefaultRoot = nullptr;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	class USplineComponent* m_Spline;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UHierarchicalInstancedStaticMeshComponent* m_MeshComponent;
	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UHierarchicalInstancedStaticMeshComponent* m_MeshComponent1;
	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UHierarchicalInstancedStaticMeshComponent* m_MeshComponent2;


	void UpdateVehicles(float DeltaTime);


	int testint = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//TArray<FTransform> m_Instances;


	bool testinit = true;
	int m_NumberOfInstances = 0;

	//100 = 1m; 
	float m_MaxLengthOfCar = 800.0f;
	//dist per s that gets added
	float m_DistToAdd = 3000.0f;
	float m_DistBetweenVehicles = 1000.0f;
	float m_DeltaCounter;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
