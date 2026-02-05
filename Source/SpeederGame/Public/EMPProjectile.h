// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunProjectile.h"
#include "Components/SphereComponent.h"
#include "EMPProjectile.generated.h"

class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class SPEEDERGAME_API AEMPProjectile : public AGunProjectile
{
	GENERATED_BODY()

	AEMPProjectile();
protected:
	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UNiagaraComponent* m_EMPNiagara;
	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	USphereComponent* m_TriggerSphere;

public:

	UPROPERTY(EditAnywhere, Category = Sound, meta = (AllowPrivateAccess))
	USoundBase* m_BlastSound;
	UFUNCTION()
	void OnBlastBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBombCompHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	bool m_AlreadyDetonated = false;

	virtual void BeginPlay() override;

	
	void Detonate();
};
