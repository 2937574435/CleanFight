
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


class AWeapon;
 

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLEANFIGHT_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponComponent();

protected:
	virtual void BeginPlay() override; 
	UPROPERTY(EditAnywhere,Category="Weapon")
	FName WeaponSocketName="WeaponSocket";

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable,Category="Weapon")
	AWeapon* GetCurrentWeapon() const; 
	UFUNCTION(BlueprintCallable,Category="Weapon")
	TArray<AWeapon*> GetWeaponList() const; 
	UFUNCTION(BlueprintCallable,Category="Weapon")
	UMaterial* GetCurrentWeaponIcon() const; 
	void Fire();
	void MakeShoot(); 
  
private:
	
	FTransform GetWeaponSocketTransform() const;
	FVector GetFireTargetPoint() const;
	FVector GetAimPoint() const;

	AController* GetOwnerController() const;

	int CurrentWeaponIndex=0;
	UPROPERTY(EditAnywhere,Category="Weapon")
	TArray<TSubclassOf<AWeapon>> DefaultWeaponClassList;
	
	TArray<AWeapon*> WeaponList;
	 

	FTimerHandle FireCDTimerHandle;
	void SpawnWeapon(); 
	bool bFireInCD=false;
	void FireCDFinish(){bFireInCD=false;}
	float FireRange=10000;
	int MaxWeaponNumber=4;
};
