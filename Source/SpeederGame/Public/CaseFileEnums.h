// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM()
enum class ELocations : uint8
{
	DEFAULT,
	CityCenter,
	District1,
	District2,
	District3,
	District4,
	District5,
	District6,
	ENUM_MAX,
};

UENUM()
enum class ERaces : uint8
{
	DEFAULT,
	Human,
	LongNeckian,
	Bluluan,
	Crickan,
	ENUM_MAX,

};

UENUM()
enum class ECrimes : uint8
{
	DEFAULT,
	Speeding,
	Robbery,
	Murder,
	Kidnapping,
	Hijacking,
	CarelessDriving,
	DrugDistribution,
	WeaponSmuggling,
	BreachOfPrison,
	DrunkPiloting,
	ENUM_MAX,


};

UENUM()
enum class ECriminalWeapons : uint8
{
	DEFAULT,
	X10_Turret,
	Z200_LaserCannons,
	NONE,
	ENUM_MAX,
};

UENUM()
enum class EFileInfoType : uint8
{
	DEFAULT,
	Age,
	Name,
	Height,
	Location,
	Race,
	Crime,
	Speed,
	Weapon,
	ENUM_MAX,
};
