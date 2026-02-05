// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GunProjectile.generated.h"

UCLASS()
class SPEEDERGAME_API AGunProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunProjectile();

	//UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	//USceneComponent* m_DefaultRoot = nullptr;

	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UStaticMeshComponent* m_ProjectileMesh;
	UPROPERTY(EditAnywhere, Category = Comps, meta = (AllowPrivateAccess))
	UProjectileMovementComponent* m_ProjectileMovement;

	UPROPERTY(EditAnywhere,BlueprintReadWrite ,Category = Stats, meta = (AllowPrivateAccess))
	float m_Damage = 0;
	UPROPERTY(EditAnywhere, Category = Stats, meta = (AllowPrivateAccess))
	bool m_IsStun = false;
	UPROPERTY(EditAnywhere, Category = Sound, meta = (AllowPrivateAccess))
	USoundBase* m_ShootSound;
	UPROPERTY(EditAnywhere, Category = Sound, meta = (AllowPrivateAccess))
	USoundBase* m_StunShootSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
