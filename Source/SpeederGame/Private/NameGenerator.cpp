// Fill out your copyright notice in the Description page of Project Settings.


#include "NameGenerator.h"

NameGenerator::NameGenerator()
{
}

NameGenerator::~NameGenerator()
{
}
//used for generating the name sof the criminals
//this is nothing really special and also does nto work that well xD
//basically just mixing vocals and consonants together, with some double letters in between ;)
FText NameGenerator::GenerateRandomName(int a_NameLength = 1, bool a_StartsWithVocal = false)
{
	FString name = "";
	bool nextIsVocal = a_StartsWithVocal;
	bool useDoubleVocal = FMath::RandBool();
	bool useDoubleConsonant = FMath::RandBool();
	bool useCombinedLetter = true;

	for (int i = 0; i < a_NameLength; i++)
	{
		if(i!=0&&useDoubleVocal&&nextIsVocal && FMath::RandRange(0,3)==1)
		{
			name += m_DoubleVocals[FMath::RandRange(0, m_DoubleVocals.Num() - 1)];
			useDoubleVocal = false;
			nextIsVocal = false;
			i++;
			continue;
		}
		if(i!=0&&useDoubleConsonant&&!nextIsVocal && FMath::RandRange(0, 2) == 1)
		{
			name += m_DoubleConsonants[FMath::RandRange(0, m_DoubleConsonants.Num() - 1)];
			useDoubleConsonant = false;
			nextIsVocal = true;
			i++;
			continue;
		}
		if (i != 0 && useCombinedLetter && !nextIsVocal&& FMath::RandRange(0, 1) == 1)
		{
			name += m_CombinedLetters[FMath::RandRange(0, m_CombinedLetters.Num() - 1)];
			useCombinedLetter = false;
			nextIsVocal = true;
			i++;
			continue;
		}
		if (nextIsVocal)
		{
			name += m_Vocals[FMath::RandRange(0, m_Vocals.Num() - 1)];
			nextIsVocal = false;
		}
		else if (!nextIsVocal)
		{
			name += m_WantedConsonants[FMath::RandRange(0, m_WantedConsonants.Num() - 1)];
			nextIsVocal = true;
		}
		if(i==0)
		{
			name = name.ToUpper();
		}
	}
	FText finishedName = FText::FromString(name);
	return finishedName;
}