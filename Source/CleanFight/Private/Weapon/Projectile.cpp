

#include "Weapon/Projectile.h"

#include "NiagaraFunctionLibrary.h" 
#include "Component/HitVFXComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogProjectile,All,All)
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	CollisionComp=CreateDefaultSubobject<USphereComponent>("CollisionComp");
	ProjectileMovement=CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	HitVfxComp=CreateDefaultSubobject<UHitVFXComponent>("HitVfxComp");

	CollisionComp->SetCollisionResponseToAllChannels(ECR_Block); 
	CollisionComp->bReturnMaterialOnMove=true;
	
	SetRootComponent(CollisionComp);
	
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->OnComponentHit.AddDynamic(this,&AProjectile::OnProjectileHit);
}

void AProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(Hit.bBlockingHit)
	{
		UE_LOG(LogProjectile,Display,TEXT("%s 射出子弹命中 %s"),*GetOwner()->GetName(),*Hit.GetActor()->GetName()); 
		if(bExplore)
		{
			Explore();
		}
		else
		{
			HitVfxComp->PlayVfxOnHit(Hit); 
			Hit.GetActor()->TakeDamage(ProjectileDamage,FDamageEvent(),nullptr,GetOwner());
		}
	}
	Destroy();
}
 

void AProjectile::Explore()
{ 
	check(GetWorld());
	UGameplayStatics::ApplyRadialDamage(GetWorld(), ProjectileDamage, GetActorLocation(), ExploreRadius, DamageType, {},
	                                    this, nullptr, bDoFullDamage);
	CollisionComp->AddRadialImpulse(GetActorLocation(), ExploreRadius, ExploreRadius, ERadialImpulseFalloff::RIF_Linear,
	                                false);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ExploreVFX,GetActorLocation());
	
}
 
