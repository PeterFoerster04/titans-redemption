// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CriminalStats.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Pawn.h"
#include "NavMeshGrid3D.h"
#include "Criminal.generated.h"


class UWidgetComponent;

UCLASS()
class SPEEDERGAME_API ACriminal : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACriminal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	USceneComponent* m_DefaultRoot = nullptr;

	UPROPERTY(EditAnywhere, Category = Sound, meta = (AllowPrivateAccess))
	USoundBase* m_HitSound;
	UPROPERTY(EditAnywhere, Category = Sound, meta = (AllowPrivateAccess))
	USoundBase* m_CrashSound;

	UPROPERTY(EditAnywhere, Category = Sound, meta = (AllowPrivateAccess))
	USoundBase* m_ShieldHitSound;
	

	UPROPERTY()
	class ACriminalController* m_Controller;

	//if criminal gets hit by stun, speed will decrease by 10% permanently 
	float m_StunNewSpeedMultiply = 0.9f;

	//if criminal gets hit, speed will decrease by 20% permanently 
	float m_HitNewSpeedMultiply = 0.8f;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void TakeProjectileDamage(float a_Damage, float* a_ValueToModify);

	void CalculateKmpH(float a_DeltaTime);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//called when player rams into criminal, chance to slow vehicle down;
	void TakeCriticalDamage(float a_Damage);

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UStaticMeshComponent* m_StaticMesh;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UBehaviorTree* m_TreeAsset;
	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UCriminalStats* m_CriminalStats;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Comps, meta = (AllowPrivateAccess))
	UWidgetComponent* m_Marker;
	UPROPERTY(BlueprintReadOnly)
	float m_CalculatedSpeedInKmph = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	bool m_WasEMPd = false;

};


