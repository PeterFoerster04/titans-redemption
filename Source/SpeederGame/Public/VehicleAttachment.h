// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VehicleAttachment.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPEEDERGAME_API UVehicleAttachment : public UStaticMeshComponent
{
	GENERATED_BODY()


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = Stats, meta = (AllowPrivateAccess))
	int m_RepairCost = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats, meta = (AllowPrivateAccess))
	int m_UpgradeCost = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats, meta = (AllowPrivateAccess))
	int m_AdjustmentCost = 0;

	UPROPERTY(EditAnywhere, Category = Stats, meta = (AllowPrivateAccess))
	FVector m_PositionOnVehicle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats, meta = (AllowPrivateAccess))
	bool m_AttachmentRepaired = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats, meta = (AllowPrivateAccess))
	bool m_AttachmentActive = false;








};
