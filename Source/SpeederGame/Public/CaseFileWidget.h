// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CaseFileEnums.h"
#include "Blueprint/UserWidget.h"
#include "CaseFileWidget.generated.h"


class UCanvasPanel;
class UTextBlock;
class UImage;

UCLASS(Abstract)
class SPEEDERGAME_API UCaseFileWidget : public UUserWidget
{
	GENERATED_BODY()


protected:

	virtual void NativeConstruct() override;
	//text blocks have to named exactly like in the widget
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* FileImage;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CaseCode;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Name;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Age;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Height;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Race;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Location;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Record;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Crimes1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Crimes2;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Speed;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Weapons1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Weapons2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Weapons3;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Extra1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Extra2;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Reward;

	template<typename CaseEnum> 
	FText ChooseDynamicEnumElement(int a_StringCutLength,bool a_IsSpecific = false,int a_SpecificIndex =0);

	FText RandomStringValueInRange(int a_Min, int a_Max,EFileInfoType a_Type = EFileInfoType::DEFAULT);

	FText RandomCaseCode();
public :
	UPROPERTY(BlueprintReadWrite)
	float m_RewardAmount = 0.0f;
	UPROPERTY(BlueprintReadWrite)
	float m_MaxSpeed = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	int m_SpawnIndex = 0;
};












