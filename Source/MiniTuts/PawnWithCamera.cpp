// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniTuts.h"
#include "PawnWithCamera.h"


// Sets default values
APawnWithCamera::APawnWithCamera()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create basic empty SceneComponent as the root of our Component hierarchy.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	//create our camera spring arm
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->AttachTo(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0, 0, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	OurCameraSpringArm->TargetArmLength = 400.f;
	OurCameraSpringArm->bEnableCameraLag = true;
	OurCameraSpringArm->CameraLagSpeed = 3.0f;
	// Create Camera and attach the camera component to the socket on the end of the SpringArmComponent
	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	OurCamera->AttachTo(OurCameraSpringArm, USpringArmComponent::SocketName);
	//Take control of the default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APawnWithCamera::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APawnWithCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Zoom in if ZoomIn button is down Zoom back out if it's not
	if (bZoomingIn)
	{
		ZoomFactor += DeltaTime / 0.5f; // Zoom in over half a second
	}
	else
	{
		ZoomFactor -= DeltaTime / .25f; // Zoom out over a quarter of a second
	}
	ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);
	//Blend our camera's FOV and our spring's arm lenght based on ZoomFactor
	OurCamera->FieldOfView = FMath::Lerp<float>(90, 60, ZoomFactor);
	OurCameraSpringArm->TargetArmLength = FMath::Lerp<float>(400, 300, ZoomFactor);
	//Rotate our actor's yaw, which will turn our camera because we're attached to it 
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += CameraInput.X;
	SetActorRotation(NewRotation);
	//Rotate our camera's pitch, but limit it so we're always looking downward
	NewRotation = OurCameraSpringArm->GetComponentRotation();
	NewRotation.Pitch = FMath::Clamp<float>(NewRotation.Pitch + CameraInput.Y, -80.0f, -15.0f);
	OurCameraSpringArm->SetWorldRotation(NewRotation);
	//Handle movement based on our "MoveX" and "MoveY" axes
	if (!MovementInput.IsZero())
	{
		//Scale our movement input axis values by 100 units per second
		MovementInput = MovementInput.SafeNormal() * 100.0f;
		FVector NewLocation = GetActorLocation();
		NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
		NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime;
		SetActorLocation(NewLocation);
	}
}

// Called to bind functionality to input
void APawnWithCamera::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	//Hook up events for "ZoomIn"
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &APawnWithCamera::ZoomIn);
	InputComponent->BindAction("ZoomIn", IE_Released, this, &APawnWithCamera::ZoomOut);

	//Hook up every-frame handling for our four axes
	InputComponent->BindAxis("MoveForward", this, &APawnWithCamera::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APawnWithCamera::MoveRight);
	InputComponent->BindAxis("CameraPitch", this, &APawnWithCamera::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &APawnWithCamera::YawCamera);
}

void APawnWithCamera::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1, 1);
}

void APawnWithCamera::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1, 1);
}

void APawnWithCamera::YawCamera(float AxisValue)
{
	CameraInput.X = FMath::Clamp<float>(AxisValue, -1, 1);
}

void APawnWithCamera::PitchCamera(float AxisValue)
{
	CameraInput.Y = FMath::Clamp<float>(AxisValue, -1, 1);
}

void APawnWithCamera::ZoomIn()
{
	bZoomingIn = true;
}

void APawnWithCamera::ZoomOut()
{
	bZoomingIn = false;
}