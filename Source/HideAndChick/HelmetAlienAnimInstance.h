// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HelmetAlienAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHit);

/**
 *
 */
UCLASS()
class HIDEANDCHICK_API UHelmetAlienAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UHelmetAlienAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayMontage();

//	void PlayClapMontage();

//	void PlayDeathMontage();

	void JumpToSection(int32 SectionIndex);

	FName GetMontageName(int32 SectionIndex);

private:
	UFUNCTION()
		void AnimNotify_AttackHit();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Montage;

//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
//		UAnimMontage* ClapMontage;

//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
//		UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float Horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float Vertical;

public:
	FOnAttackHit OnAttackHit;
};
