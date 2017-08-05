// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlerGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

UPuzzlerGameInstance::UPuzzlerGameInstance()
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuClassFinder(TEXT("/Game/UI/WBP_MainMenu")); //Must be in constructor.
	MenuClass = MenuClassFinder.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> JoinMenuClassFinder(TEXT("/Game/UI/WBP_JoinServerMenu")); //Must be in constructor.
	JoinMenuClass = JoinMenuClassFinder.Class;
}

void UPuzzlerGameInstance::Init()
{
	Super::Init();
}

void UPuzzlerGameInstance::LoadMainMenu()
{
	if (!ensure(MenuClass != nullptr)) return;

	CurrentlyActiveMenu = CreateWidget<UUserWidget>(this, MenuClass);

	ShowMenu();

}

void UPuzzlerGameInstance::LoadJoinServerMenu()
{
	if (!ensure(JoinMenuClass != nullptr)) return;

	HideMenu();
	CurrentlyActiveMenu = CreateWidget<UUserWidget>(this, JoinMenuClass);
	ShowMenu();
}

void UPuzzlerGameInstance::HostServer()
{
	// Note: the listen is important.
	GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");

	HideMenu();
}


void UPuzzlerGameInstance::ShowMenu()
{
	if (!ensure(CurrentlyActiveMenu != nullptr)) return;

	CurrentlyActiveMenu->AddToViewport();

	// Step 0 get player controller.
	auto PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	// Step 2 intellisense which mode options there are.
	FInputModeUIOnly InputModeData;
	// Step 3 which configure options:
	InputModeData.SetLockMouseToViewport(false);
	InputModeData.SetWidgetToFocus(CurrentlyActiveMenu->TakeWidget()); //Because UMG wraps Slate
																	   // Step 1 what to give input mode:
	PlayerController->SetInputMode(InputModeData);

	// Step 4 enable cursor:
	PlayerController->bShowMouseCursor = true;
}

void UPuzzlerGameInstance::HideMenu()
{
	if (!ensure(CurrentlyActiveMenu != nullptr)) return;

	CurrentlyActiveMenu->RemoveFromViewport();

	auto PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = false;
}
