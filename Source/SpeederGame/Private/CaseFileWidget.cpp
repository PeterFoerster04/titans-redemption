// Fill out your copyright notice in the Description page of Project Settings.


#include "CaseFileWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "NameGenerator.h"
#include "Components/Image.h"



void UCaseFileWidget::NativeConstruct()
{
	Super::NativeConstruct();


	if (!CanvasPanel) return;
	
	CaseCode->SetText(RandomCaseCode());

	// setting all the information, some of the values are retrieved in blueprints so do not wonder

	Name->SetText(NameGenerator::GenerateRandomName(FMath::RandRange(4, 10),FMath::RandBool()));
	Age->SetText(RandomStringValueInRange(18,80,EFileInfoType::Age));
	Height->SetText(RandomStringValueInRange(30,99, EFileInfoType::Height));
	
	Race->SetText(ChooseDynamicEnumElement<ERaces>(8));
	Location->SetText(ChooseDynamicEnumElement<ELocations>(12,true,m_SpawnIndex));
	
	Crimes1->SetText(ChooseDynamicEnumElement<ECrimes>(9));
	Crimes2->SetText(ChooseDynamicEnumElement<ECrimes>(9));

	Speed->SetText(FText::FromString(FString::FromInt(m_MaxSpeed) + FString(" km/h")));
	

	Weapons1->SetText(ChooseDynamicEnumElement<ECriminalWeapons>(18));

	Reward->SetText(FText::FromString(FString::FromInt(m_RewardAmount)+ FString(" C")));

}

FText UCaseFileWidget::RandomStringValueInRange(int a_Min, int a_Max, EFileInfoType a_Type)
{

	FString start;
	FString value;
	FString end;

	switch (a_Type)
	{
		case EFileInfoType::Age:
			value += FString::FromInt(FMath::RandRange(a_Min, a_Max));
			value += " Years";
			break;
		case EFileInfoType::Height:
			value += "1,";
			value += FString::FromInt(FMath::RandRange(a_Min, a_Max));
			value += "m";
			break;
		case EFileInfoType::Speed:
			value += FString::FromInt(FMath::RandRange(a_Min, a_Max));
			value += " km/h";
			break;
		default:
			value += FString::FromInt(FMath::RandRange(a_Min, a_Max));
			break;
	}

	return FText::FromString(value);
}

FText UCaseFileWidget::RandomCaseCode()
{
	FString code = "X";
	code += FString::FromInt(FMath::RandRange(1000, 9999));
	return FText::FromString(code);
}

//function to get information out of enums and converting them into text
template <typename CaseEnum>
FText UCaseFileWidget::ChooseDynamicEnumElement(int a_StringCutLength, bool a_IsSpecific, int a_SpecificIndex)
{
	FString enumValue;
	if(a_IsSpecific)
	{
		enumValue = UEnum::GetValueAsString(static_cast<CaseEnum>(a_SpecificIndex+1));//skipping default value
		enumValue.RemoveAt(0, a_StringCutLength);
		return FText::FromString(enumValue);
	}
	
	//the cut length ensures that the final string does nor contain useless information such as the enum name
	enumValue = UEnum::GetValueAsString(static_cast<CaseEnum>(FMath::RandRange(1, static_cast<int>(CaseEnum::ENUM_MAX) - 1)));
	enumValue.RemoveAt(0, a_StringCutLength);
	return FText::FromString(enumValue);
	
}
