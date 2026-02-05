// Fill out your copyright notice in the Description page of Project Settings.
//
#pragma once

#include "CoreMinimal.h"
#include "GunProjectile.h"
#include "GameFramework/Pawn.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/SpringArmComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TitanHook.h"
#include "TitanStunTurret.h"
#include "CableComponent.h"
#include "EMPProjectile.h"
#include "PlayerStats.h"
#include "TitanEMPCannon.h"
#include "TitanMissileLauncher.h"
#include "VehicleAttachment.h"
#include "Aircraft.generated.h"


class UPostProcessComponent;
class UNiagaraComponent;

UCLASS()
class SPEEDERGAME_API AAircraft : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAircraft();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//Scene Components
	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	USceneComponent* m_DefaultRoot = nullptr;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	USpringArmComponent* m_SpringArm = nullptr;

	UPROPERTY(EditAnywhere,Category = Comps, meta= (AllowPrivateAccess))
	class UCameraComponent* m_Camera = nullptr;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UStaticMeshComponent* m_Mesh = nullptr;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UPostProcessComponent* m_PostP;

	//MiniMap 
	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	USpringArmComponent* m_MiniMapSpringArm = nullptr;


	UPROPERTY()
	UWorld* m_World;

	//Projectile Spawn Positions
	UPROPERTY(EditAnywhere, Category = Attachments, meta = (AllowPrivateAccess))
	USceneComponent* m_GunLeftFirePos;
	UPROPERTY(EditAnywhere, Category = Attachments, meta = (AllowPrivateAccess))
	USceneComponent* m_GunRightFirePos;

	UPROPERTY(EditAnywhere, Category = Attachments, meta = (AllowPrivateAccess))
	USceneComponent* m_TurretLeftFirePos;
	UPROPERTY(EditAnywhere, Category = Attachments, meta = (AllowPrivateAccess))
	USceneComponent* m_TurretRightFirePos;

	UPROPERTY(EditAnywhere, Category = Attachments, meta = (AllowPrivateAccess))
	USceneComponent* m_MissileLeftFirePos;
	UPROPERTY(EditAnywhere, Category = Attachments, meta = (AllowPrivateAccess))
	USceneComponent* m_MissileRightFirePos;
	UPROPERTY(EditAnywhere, Category = Attachments, meta = (AllowPrivateAccess))
	USceneComponent* m_EMPFirePos;

	//Niagara Systems
	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UNiagaraComponent* m_BoosterRNS;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UNiagaraComponent* m_BoosterLNS;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UNiagaraComponent* m_ThrusterRNS;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UNiagaraComponent* m_ThrusterLNS;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UNiagaraComponent* m_SpeedLinesNS;

	//Input 
	UPROPERTY(EditAnywhere, Category = Input,meta = (AllowPrivateAccess))
	UInputMappingContext* mappingContext;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess))
	UInputAction* m_YawAction;
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess))
	UInputAction* m_RollAction;
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess))
	UInputAction* m_PitchAction;
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess))
	UInputAction* m_ThrustAction;
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess))
	UInputAction* m_BoostAction;
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess))
	UInputAction* m_ToggleLightsAction;
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess))
	UInputAction* m_GunShootAction;
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess))
	UInputAction* m_TurretShootAction;
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess))
	UInputAction* m_MissileLaunchAction;
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess))
	UInputAction* m_HookAction;
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess))
	UInputAction* m_GearShiftAction;
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess))
	UInputAction* m_ToggleCurrentCaseFile;
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess))
	UInputAction* m_ShootEMPAction;

	UPROPERTY()
	UEnhancedInputComponent* m_EnhancedInput;

	//Sounds
	UPROPERTY(EditAnywhere, Category = Sounds, meta = (AllowPrivateAccess))
	USoundBase* m_HookShootSound;

	UPROPERTY(EditAnywhere, Category = Sounds, meta = (AllowPrivateAccess))
	USoundBase* m_HookAttachSound;

	UPROPERTY(EditAnywhere, Category = Sounds, meta = (AllowPrivateAccess))
	USoundBase* m_HookRetractSound;

	UPROPERTY(EditAnywhere, Category = Sounds, meta = (AllowPrivateAccess))
	USoundBase* m_BoostQuantumSound;

	UPROPERTY(EditAnywhere, Category = Sounds, meta = (AllowPrivateAccess))
	USoundBase* m_BoosterPowerUpSound;
	UPROPERTY(EditAnywhere, Category = Sounds, meta = (AllowPrivateAccess))
	USoundBase* m_BoosterPowerDownSound;


	UPROPERTY(EditAnywhere, Category = Sounds, meta = (AllowPrivateAccess))
	USoundBase* m_CrashSound;

	UPROPERTY()
	UAudioComponent* m_BoosterAudioComp;
	
	bool m_SpawnedBoosterAudio = false;

	//Projectiles
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Projectiles, meta = (AllowPrivateAccess))
	TSubclassOf<AGunProjectile> m_GunProjectile;

	UPROPERTY(EditAnywhere, Category = Projectiles, meta = (AllowPrivateAccess))
	TSubclassOf<AGunProjectile> m_StunProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectiles, meta = (AllowPrivateAccess))
	TSubclassOf<AGunProjectile> m_Missile;

	UPROPERTY(EditAnywhere, Category = Projectiles, meta = (AllowPrivateAccess))
	TSubclassOf<AEMPProjectile> m_EMPProjectile;


	UPROPERTY()
	AEMPProjectile* m_FiredEMPBomb = nullptr;

	//Blueprint Parameters

	UPROPERTY(EditAnywhere, Category = Boost, meta = (AllowPrivateAccess))
	float m_BoostMultiplier = 2.0f;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess))
	float m_MaxMouseInput = 1.5f;


	UPROPERTY(BlueprintReadOnly)
	float m_Speedkmph = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	float m_CurrentGear = 1.0f;
	UPROPERTY(BlueprintReadOnly)
	float m_MaxGear = 5.0f;
	UPROPERTY(BlueprintReadOnly)
	float m_BoostCharge = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	float m_MaxBoostCharge = 100.0f;

	UPROPERTY(EditAnywhere, Category = Boost, meta = (AllowPrivateAccess))
	float m_BoostConsumption = 15.0f;

	UPROPERTY(EditAnywhere, Category = Boost, meta = (AllowPrivateAccess))
	float m_BoostRechargeAmount = 5.0f;
	UPROPERTY(BlueprintReadOnly)
	float m_BoostCooldown = 5.0f;
	UPROPERTY(BlueprintReadOnly)
	bool m_BoostIsOnCooldown = false;


	int m_MaxBlasterShots = 20;
	UPROPERTY(BlueprintReadOnly)
	int m_BlasterShotsLeft = 0;
	float m_BlasterRechargeTime = 4.0f;
	UPROPERTY(BlueprintReadOnly)
	float m_CurrentBlasterRechargeTime = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float m_Thrust = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	bool m_Boosting = false;
	bool m_BoosterIsCancelable = false;
	float m_CurrentBoostMultiply = 1.0f;

	UPROPERTY(EditAnywhere, Category = Stats, meta = (AllowPrivateAccess))
	float m_ThrustSpeed = 6000.0f;
	float m_ShakeAmount = 300.0f;

	UPROPERTY(BlueprintReadWrite)
	float m_MouseSens = 1.0f;
	float m_RollMultiplier = 10.0f;
	float m_YawMultiplier = 5.0f;
	float m_PitchMultiplier = 5.0f;

	float m_DefaultFOV = 100.0f;


	FVector m_SpringArmInitPos;

	bool m_PlayedHookAttachSound = false;

	bool m_HookIsExtended = false;
	bool m_RetractHook = false;
	bool m_ActivatedHook = false;
	UPROPERTY(BlueprintReadOnly)
	bool m_HookedCriminal = false;

	UPROPERTY(BlueprintReadOnly)
	float m_CurrentHookHack = 0.0f;
	float m_HookHackTime = 8.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Attachments, meta = (AllowPrivateAccess))
	bool m_RepairedBooster = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attachments, meta = (AllowPrivateAccess))
	bool m_BoosterActive = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attachments, meta = (AllowPrivateAccess))
	int m_BoosterRepairCost = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attachments, meta = (AllowPrivateAccess))
	int m_BoosterACCost = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attachments, meta = (AllowPrivateAccess))
	bool m_GunsActive = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attachments, meta = (AllowPrivateAccess))
	int m_GunsACCost = 0;


	//Attachments
	UPROPERTY(EditAnywhere, Category = Attachments, meta = (AllowPrivateAccess))
	TArray<UVehicleAttachment*> m_AllAttachments;

	//Hook Stuff
	UPROPERTY(EditAnywhere, Category = Attachments, meta = (AllowPrivateAccess))
	UTitanHook* m_Hook;
	UPROPERTY(EditAnywhere, Category = Attachments, meta = (AllowPrivateAccess))
	UCableComponent* m_HookCable;
	UPROPERTY(EditAnywhere, Category = Attachments, meta = (AllowPrivateAccess))
	USceneComponent* m_HookInitPos;
	FHitResult m_HookImpactPoint;

	//Stun Turret
	UPROPERTY(EditAnywhere, Category = Attachments, meta = (AllowPrivateAccess))
	UTitanStunTurret* m_StunTurret;

	//Missile Launcher
public:
	UPROPERTY(EditAnywhere, Category = Attachments, meta = (AllowPrivateAccess))
	UTitanMissileLauncher* m_LeftMissileLauncher;
	UPROPERTY(EditAnywhere, Category = Attachments, meta = (AllowPrivateAccess))
	UTitanMissileLauncher* m_RightMissileLauncher;

	UPROPERTY(EditAnywhere, Category = Attachments, meta = (AllowPrivateAccess))
	UTitanEMPCannon* m_EMPCannon;
protected:

	UFUNCTION()
	void OnHookCompHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


public:
	UFUNCTION(BlueprintCallable)
	void PortPlayerBackToSpawn(FVector a_SpawnPos);


	UFUNCTION(BlueprintImplementableEvent)
	void ExplosionCamShake();
protected:
	//Member Functions
	void ChangeBoosterMaterialGlow(float a_RedValue, float a_YellowValue);
	void AddMovementForce(float a_DeltaTime);
	void ShakeCamera(float a_DeltaTime);
	void CalculateKmpH(float a_DeltaTime);
	void RotateMinimap();
	void HandleHook(float a_DeltaTime);
	void RetractHook();
	void UpdateFlightVisualFeedback(float a_DeltaTime);
	void TimePlayerHookCriminal(float a_DeltaTime);
	void TickEMPTimer(float a_DeltaTime);

	void UpdateBoostCharge(float a_DeltaTime);
	void ConsumeBoostCharge(float a_DeltaTime);
	void DeactivateBoosterSystems();
	void RechargeBlasters(float a_DeltaTime);
	void ManagePlayerLocation();

	//Callback Functions for Input
	void RollInput(const FInputActionValue& a_Value);
	void YawInput(const FInputActionValue& a_Value);
	void PitchInput(const FInputActionValue& a_Value);
	void ThrustInput(const FInputActionValue& a_Value);
	void BoostInput(const FInputActionValue& a_Value);
	void ToggleLightsInput(const FInputActionValue& a_Value);
	void GunShoot(const FInputActionValue& a_Value);
	void TurretShoot(const FInputActionValue& a_Value);
	void ShootHook(const FInputActionValue& a_Value);
	void LaunchMissiles(const FInputActionValue& a_Value);
	void GearShift(const FInputActionValue& a_Value);
	void ToggleCurrentCaseFileOnScreen(const FInputActionValue& a_Value);
	void ShootEMP(const FInputActionValue& a_Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool ParkVehicleInGarage(FVector a_Loc, FRotator a_Rot, float a_DeltaTime);
	void EjectVehicleOutside(FVector a_Loc, FRotator a_Rot);


	bool m_HookAimLockActive = false;
	FRotator m_HookAimLockDirection;

	int testint = 5;

	//Vehicle blueprint variables
	UPROPERTY(BlueprintReadWrite)
	bool m_InputBlocked = false;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UPlayerStats* m_Stats;
	UPROPERTY(BlueprintReadOnly, Category = Widgets, meta = (AllowPrivateAccess))
	bool m_CaseFileWidgetOnScreen = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Stats, meta = (AllowPrivateAccess))
	float m_VehicleMaxHP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Stats, meta = (AllowPrivateAccess))
	float m_VehicleMaxShields = 0;

	UPROPERTY(BlueprintReadOnly)
	float m_VehicleCurrentHP = 0;
	UPROPERTY(BlueprintReadOnly)
	float m_VehicleCurrentShields = 0;

	UPROPERTY(BlueprintReadWrite)
	bool m_IsOnCrashDelay = false;
	UPROPERTY(BlueprintReadWrite)
	bool m_PlayerOutOfMapRange = false;

	UPROPERTY(BlueprintReadWrite)
	bool m_AimAssistActive = true;
};
