// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameFramework/GameMode.h"
#include "MiniTutsGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MINITUTS_API AMiniTutsGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	/** Remove the current menu widget and create a new one from the specified class if provided. */
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);
	
protected:
	/** The widget class we will use as our menu when the game starts */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	/** The widget instance we are using as our menu */
	UPROPERTY()
	UUserWidget* CurrentWidget;

};
