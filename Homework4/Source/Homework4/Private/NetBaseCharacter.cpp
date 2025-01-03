// Fill out your copyright notice in the Description page of Project Settings.


#include "NetBaseCharacter.h"
#include "NetGameInstance.h"
#include "NetPlayerState.h"

static UDataTable* SBodyParts = nullptr;

const static wchar_t* BodyPartNames[] =
{
	TEXT("Face"),
	TEXT("Hair"),
	TEXT("Chest"),
	TEXT("Hands"),
	TEXT("Legs"),
	TEXT("Beard"),
	TEXT("Eyes")
};

// Sets default values
ANetBaseCharacter::ANetBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PartFace = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	PartFace->SetupAttachment(GetMesh());
	PartFace->SetLeaderPoseComponent(GetMesh());

	PartHands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));
	PartHands->SetupAttachment(GetMesh());
	PartHands->SetLeaderPoseComponent(GetMesh());

	PartLegs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	PartLegs->SetupAttachment(GetMesh());
	PartLegs->SetLeaderPoseComponent(GetMesh());

	PartHair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hair"));
	PartHair->SetupAttachment(PartFace, FName("headSocket"));

	PartBeard = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Beard"));
	PartBeard->SetupAttachment(PartFace, FName("headSocket"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_Eyes(TEXT("StaticMesh'/Game/StylizedModularChar/Meshes/SM_Eyes.SM_Eyes'"));

	PartEyes = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eyes"));
	PartEyes->SetupAttachment(PartFace, FName("headSocket"));
	PartEyes->SetStaticMesh(SK_Eyes.Object);

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_BodyParts(TEXT("DataTable'/Game/Blueprints/DT_BodyParts.DT_BodyParts'"));
	SBodyParts = DT_BodyParts.Object;

}

// Called when the game starts or when spawned
void ANetBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GetNetMode() == NM_Standalone) return;
	SetActorHiddenInGame(true);
	CheckPlayerState();
	
}

void ANetBaseCharacter::OnConstruction(const FTransform& Transform)
{
	UpdateBodyParts();
}

// Called every frame
void ANetBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANetBaseCharacter::ChangeBodyPart(EBodyPart Index, int Value, bool DirectSet)
{
	FSMeshAssetList* List = GetBodyPartList(Index, BodyPartIndices[(int)EBodyPart::BP_BodyType] !=0);
	if (List == nullptr) return;

	int CurrentIndex = BodyPartIndices[(int)Index];

	if(DirectSet) CurrentIndex = Value;
	else
	{
		CurrentIndex += Value;
	}

	int Num = List->ListSkeletal.Num() + List->ListStatic.Num();

	if (CurrentIndex < 0)
		CurrentIndex += Num;
	else
		CurrentIndex %= Num;

	BodyPartIndices[(int)Index] = CurrentIndex;

	switch(Index)
	{
	case EBodyPart::BP_Face:PartFace->SetSkeletalMeshAsset(List->ListSkeletal[CurrentIndex]); break;
	case EBodyPart::BP_Beard:PartBeard->SetStaticMesh(List->ListStatic[CurrentIndex]); break;
	case EBodyPart::BP_Chest:GetMesh()->SetSkeletalMeshAsset(List->ListSkeletal[CurrentIndex]); break;
	case EBodyPart::BP_Hair:PartHair->SetStaticMesh(List->ListStatic[CurrentIndex]); break;
	case EBodyPart::BP_Hands:PartHands->SetSkeletalMeshAsset(List->ListSkeletal[CurrentIndex]); break;
	case EBodyPart::BP_Legs:PartLegs->SetSkeletalMeshAsset(List->ListSkeletal[CurrentIndex]); break;
	}
}

void ANetBaseCharacter::ChangeGender(bool IsFemale)
{
	if (IsFemale) PartBeard->SetStaticMesh(nullptr);
	else
	{
		ChangeBodyPart(EBodyPart::BP_Beard, 0, true);
	}
	UpdateBodyParts();
}

void ANetBaseCharacter::SubmitPlayerInfoToServer_Implementation(FSPlayerInfo Info)
{
	ANetPlayerState* State = GetPlayerState<ANetPlayerState>();
	State->Data.Nickname = Info.Nickname;
	State->Data.CustomizationData = Info.CustomizationData;
	State->Data.TeamID = State->TeamID;
	PlayerInfoReceived = true;
}
FString ANetBaseCharacter::GetCustomizationData() const
{
	FString Data;
	for (size_t i = 0; i < static_cast<int>(EBodyPart::BP_COUNT); i++)
	{
		Data += FString::FromInt(BodyPartIndices[i]);
		if (i < (static_cast<int>(EBodyPart::BP_COUNT) - 1))
		{
			Data += TEXT(",");
		}
	}
	return Data;
}

void ANetBaseCharacter::ParseCustomizationData(FString BodyPartData)
{
	TArray<FString> ArrayData;
	BodyPartData.ParseIntoArray(ArrayData, TEXT(","));
	for (size_t i = 0; i < ArrayData.Num(); i++)
	{
		BodyPartIndices[i] = FCString::Atoi(*ArrayData[i]);
	}
}

void ANetBaseCharacter::CheckPlayerState()
{
	ANetPlayerState* State = GetPlayerState<ANetPlayerState>();

	if (State == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("State == nullptr"));

		GWorld->GetTimerManager().SetTimer(ClientDataCheckTimer, this, &ANetBaseCharacter::CheckPlayerState, 0.25f,false);
	}
	else
	{
		if (IsLocallyControlled())
		{
			UNetGameInstance* Instance = Cast<UNetGameInstance>(GWorld->GetGameInstance());
			if (Instance) SubmitPlayerInfoToServer(Instance->PlayerInfo);
		}
		CheckPlayerInfo();
	}
}

void ANetBaseCharacter::CheckPlayerInfo()
{
	ANetPlayerState* State = GetPlayerState<ANetPlayerState>();

	if (State && PlayerInfoReceived)
	{
		ParseCustomizationData(State->Data.CustomizationData);
		UpdateBodyParts();
		OnPlayerInfoChanged();
		SetActorHiddenInGame(false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("State Not Received!"));

		GWorld->GetTimerManager().SetTimer(ClientDataCheckTimer, this, &ANetBaseCharacter::CheckPlayerInfo, 0.25f, false);
	}
}

// Called to bind functionality to input
void ANetBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FSMeshAssetList* ANetBaseCharacter::GetBodyPartList(EBodyPart Part, bool IsFemale)

{
	FString Name = FString :: Printf(TEXT("%s%s"), IsFemale ? TEXT("Female") : TEXT("Male"), BodyPartNames[(int)Part]);
	return SBodyParts ? SBodyParts->FindRow<FSMeshAssetList>(*Name, nullptr) : nullptr;
}

void ANetBaseCharacter::UpdateBodyParts()
{
	ChangeBodyPart(EBodyPart::BP_Face, 0, false);
	ChangeBodyPart(EBodyPart::BP_Beard, 0, false);
	ChangeBodyPart(EBodyPart::BP_Chest, 0, false);
	ChangeBodyPart(EBodyPart::BP_Hair, 0, false);
	ChangeBodyPart(EBodyPart::BP_Hands, 0, false);
	ChangeBodyPart(EBodyPart::BP_Legs, 0, false);
}

