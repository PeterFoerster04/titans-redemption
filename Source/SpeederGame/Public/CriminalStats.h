// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CriminalStats.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPEEDERGAME_API UCriminalStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCriminalStats();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;



	UPROPERTY(EditAnywhere, Category = Stats, meta = (AllowPrivateAccess))
	float m_VehicleMaxHP = 0;

	UPROPERTY(EditAnywhere, Category = Stats, meta = (AllowPrivateAccess))
	float m_VehicleMaxShields = 0;

	

	UPROPERTY(EditAnywhere, Category = Stats, meta = (AllowPrivateAccess))
	float m_RecordedSpeed;




public:

	UPROPERTY(EditAnywhere, Category = Stats, meta = (AllowPrivateAccess))
	float m_SpeedMultiplier;

	UPROPERTY(BlueprintReadOnly)
	float m_VehicleCurrentHP = 0;
	UPROPERTY(BlueprintReadOnly)
	float m_VehicleCurrentShields = 0;




	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	inline float GetMaxHP() { return m_VehicleMaxHP; }
	inline float GetMaxShields() { return m_VehicleMaxShields; }
	
		
};
