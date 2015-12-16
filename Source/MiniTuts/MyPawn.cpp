// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniTuts.h"
#include "MyPawn.h"


// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurMeshComponent"));
	//Attach our camera and visible object to our root component. Offset and rotate the camera
	OurCamera->AttachTo(RootComponent);
	OurCamera->SetRelativeLocation(FVector(-250.0f, 0, 250));
	OurCamera->SetRelativeRotation(FRotator(-45, 0, 0));
	OurVisibleComponent->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	float CurrentScale = OurVisibleComponent->GetComponentScale().X;
	if (bGrowing)
	{
		// Grow to double size over the course of one second
		CurrentScale += DeltaTime;
	}
	else
	{
		// Shrink half as fast as we grow
		CurrentScale -= (DeltaTime * .5f);
	}
	// make sure we never drop our staring size, or increase past double size.
	CurrentScale = FMath::Clamp(CurrentScale, 1.0f, 2.0f);
	OurVisibleComponent->SetWorldScale3D(FVector(CurrentScale));

	// Handle movement based on our MoveX and MoveY axes
	if (!CurrentVelocity.IsZero())
	{
		FVector NewLocation = GetActorLocation() + (CurrentVelocity*DeltaTime);
		SetActorLocation(NewLocation);
	}
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAxis("MoveForward", this, &AMyPawn::MoveXAxis);
	InputComponent->BindAxis("MoveRight", this, &AMyPawn::MoveYAxis);

	InputComponent->BindAction("Space", IE_Pressed, this, &AMyPawn::StartGrowing);
	InputComponent->BindAction("Space", IE_Released, this, &AMyPawn::StopGrowing);
}

void AMyPawn::MoveXAxis(float AxisValue)
{
	//move at 100 units per second forward or backward
	CurrentVelocity.X = FMath::Clamp<float>(AxisValue, -1, 1) * 100;
}

void AMyPawn::MoveYAxis(float AxisValue)
{
	//move at 100 units per second right or left
	CurrentVelocity.Y = FMath::Clamp<float>(AxisValue, -1, 1) * 100;
}

void AMyPawn::StartGrowing()
{
	bGrowing = true;
}

void AMyPawn::StopGrowing()
{
	bGrowing = false;
}