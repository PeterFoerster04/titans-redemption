// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PoliceStationUIManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPEEDERGAME_API UPoliceStationUIManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPoliceStationUIManager();

	
	UPROPERTY(EditAnywhere, Category = UI, meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> m_GarageUI;
	UPROPERTY(EditAnywhere, Category = UI, meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> m_AdjustmentUI;
	UPROPERTY(EditAnywhere, Category = UI, meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> m_PoliceMissionUI;

	UPROPERTY(EditAnywhere, Category = UI, meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> m_PlayerHUD;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> m_WinScreen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> m_FailScreen;

	//getting the player hud from game mode
	UPROPERTY()
	UUserWidget* m_PlayerHUDWidget;
	UPROPERTY()
	UUserWidget* m_GarageUIWidget;
	UPROPERTY()
	UUserWidget* m_AdjustmentUIWidget;
	UPROPERTY()
	UUserWidget* m_MissionUIWidget;


	UPROPERTY(BlueprintReadOnly)
	float testFloat = 56.0f;


	UPROPERTY()
	UWorld* m_World;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool m_GotPlayerHUD = false;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SwapToGarageMenu();
	UFUNCTION(BlueprintCallable)
	void SwapToAdjustmentMenu();
	UFUNCTION(BlueprintCallable)
	void SwapToPlayerHUD();
	UFUNCTION(BlueprintCallable)
	void SwapToMissionSelectMenu();
	void ClearCurrentUI();
	
};




