// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CaseFile.h"
#include "CaseMission.h"
#include "Components/ActorComponent.h"
#include "PlayerStats.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPEEDERGAME_API UPlayerStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStats();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Stats, meta = (AllowPrivateAccess))
	int m_CurrentMoney =0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats, meta = (AllowPrivateAccess))
	int m_CurrentAC = 0;

	

	UPROPERTY(BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess))
	ACaseMission* m_CurrentMission = nullptr;

	UPROPERTY(BlueprintReadOnly)
		bool m_IsCurrentlyInMission = false;

		bool m_HackedCriminalVehicle = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	int testint = 123;
		
};
