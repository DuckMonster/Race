#include "BoostRefill.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Race/Car/RaceCar.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

ABoostRefill::ABoostRefill()
{
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	RootComponent = HitBox;

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &ABoostRefill::HandleOverlap);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	DecalComp->SetupAttachment(RootComponent);
}

void ABoostRefill::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, TEXT("COLLIDED"));
	}
	ARaceCar* Player = Cast<ARaceCar>(OtherActor);
	if (Player)
	{
		Player->BoostTime = 2.f;
		APlayerController* PlayerController;
		PlayerController = Player->GetController()->CastToPlayerController();
		int PlayerID = UGameplayStatics::GetPlayerControllerID(PlayerController);

		if (DecalComp)
		{
			FLinearColor color = PlayerID == 0 ? FLinearColor::Blue : FLinearColor::Red;

			MatInst->SetVectorParameterValue(TEXT("CircleColor"), color);
			DecalComp->SetDecalMaterial(MatInst);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, TEXT("NO DECAL FOUND"));
		}
	}
}

void ABoostRefill::BeginPlay()
{
	Super::BeginPlay();

	MatInst = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, Parent);
}
