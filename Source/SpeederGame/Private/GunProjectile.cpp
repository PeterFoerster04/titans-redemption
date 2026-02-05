// Fill out your copyright notice in the Description page of Project Settings.


#include "GunProjectile.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AGunProjectile::AGunProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//m_DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	//SetRootComponent(m_DefaultRoot);

	m_ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(m_ProjectileMesh);

	m_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	

}

// Called when the game starts or when spawned
void AGunProjectile::BeginPlay()
{
	Super::BeginPlay();
	//since multiple blueprints exist from this class checking if instance is stun or not
	if(!m_IsStun &&m_ShootSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_ShootSound, GetActorLocation(), GetActorRotation());
	}
	if(m_IsStun&& m_StunShootSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_StunShootSound, GetActorLocation(), GetActorRotation());
	}
	
	
}

// Called every frame
void AGunProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

