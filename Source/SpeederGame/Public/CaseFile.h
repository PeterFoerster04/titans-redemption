// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CaseFile.generated.h"



class UWidgetComponent;
UCLASS()
class SPEEDERGAME_API ACaseFile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACaseFile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	USceneComponent* m_DefaultRoot = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Comps, meta = (AllowPrivateAccess))
	UWidgetComponent* m_CaseFileWidget;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void CustomValuesToWidget();

	void SetCustomFileValues(float a_Reward, float a_Speed, float a_SpeedMultiply,int a_SpawnIndex);

	UPROPERTY(BlueprintReadOnly)
	float m_RewardAmount = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	float m_MaxSpeed = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	int m_SpawnIndex = 0;
	float m_CriminalSpeedMultiplier = 0.0f;
};
