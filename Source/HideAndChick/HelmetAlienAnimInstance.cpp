// Fill out your copyright notice in the Description page of Project Settings.


#include "HelmetAlienAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "HelmetAlienCharacter.h"

UHelmetAlienAnimInstance::UHelmetAlienAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("AnimMontage'/Game/Animations/Alien_Helmet_Skeleton_Montage.Alien_Helmet_Skeleton_Montage'"));
	if (AM.Succeeded())
	{
		Montage = AM.Object;
	}
}

void UHelmetAlienAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Speed = Pawn->GetVelocity().Size();

		auto Character = Cast<AHelmetAlienCharacter>(Pawn);
		if (Character)
		{
			IsFalling = Character->GetMovementComponent()->IsFalling();

			Vertical = Character->UpDownValue;
			Horizontal = Character->LeftRightValue;
		}
	}
}

void UHelmetAlienAnimInstance::PlayMontage()
{
	Montage_Play(Montage, 1.f);
}

//void UHelmetAlienAnimInstance::PlayClapMontage()
//{
//	Montage_Play(ClapMontage, 1.f);
//}

//void UHelmetAlienAnimInstance::PlayDeathMontage()
//{
//	Montage_Play(DeathMontage, 1.f);
//}

void UHelmetAlienAnimInstance::JumpToSection(int32 SectionIndex)
{
	FName Name = GetMontageName(SectionIndex);
	Montage_JumpToSection(Name, Montage);
}

FName UHelmetAlienAnimInstance::GetMontageName(int32 SectionIndex)
{
	return FName(*FString::Printf(TEXT("%d"), SectionIndex));
}

void UHelmetAlienAnimInstance::AnimNotify_AttackHit()
{
	//UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackHit"));
	OnAttackHit.Broadcast();
}
