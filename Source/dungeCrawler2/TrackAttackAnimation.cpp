// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "TrackAttackAnimation.h"

void UTrackAttackAnimation::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (APlayerCharacter* character = Cast<APlayerCharacter>(MeshComp->GetOwner())) {

	}
}
