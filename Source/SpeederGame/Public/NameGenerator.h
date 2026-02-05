// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CaseFileEnums.h"
#include "CoreMinimal.h"


class SPEEDERGAME_API NameGenerator
{

private:
	static inline TArray<FString> m_Vocals = { "a","e","i","o","u" };
	static inline TArray<FString> m_WantedConsonants = { "b","c","d","f","g","h","j","k","l","m","n","p","q","r","s","t","v","w" };

	static inline TArray<FString> m_DoubleVocals = { "ee","oo","aa" };
	static inline TArray<FString> m_DoubleConsonants = { "tt","ll","pp","gg","mm","nn","bb","rr","ss"};

	static inline TArray<FString> m_CombinedLetters = { "sh","ch","ck","gh","th","rn","cl","cr","ns","ct","tr","sp","pr","st","ng","nt","nd","cc"};


	

public:
	NameGenerator();
	~NameGenerator();

	static FText GenerateRandomName(int a_NameLength, bool a_StartsWithVocal);




};
