// Fill out your copyright notice in the Description page of Project Settings.


#include "projectile.h"
#include "Components/SphereComponent.h"
#include "MyProject3Character.h"
#include "DamageDummy.h"

// Sets default values
Aprojectile::Aprojectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);  // Ignore the owner (player character)
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);  // Ignore the player
	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	RootComponent = CollisionComponent;

	Character = CreateDefaultSubobject<AMyProject3Character>(TEXT("HealthComponent"));

	// Set up the OnHit event
	CollisionComponent->OnComponentHit.AddDynamic(this, &Aprojectile::OnHit);


	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));
	ProjectileComponent->InitialSpeed = 800;
	ProjectileComponent->MaxSpeed = 8500;

	
}

// Called when the game starts or when spawned
void Aprojectile::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile Mesh Created: %s"), *ProjectileMesh->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Projectile Mesh not found!"));
	}

	
	
}

// Called every frame
void Aprojectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, ADamageDummy::StaticClass());  // Only check for DamageDummy actors

	for (AActor* Actor : OverlappingActors)
	{
		ADamageDummy* Dummy = Cast<ADamageDummy>(Actor);

		float Damage = Character->Damage;

		if (Dummy)
		{
			// Apply damage to the dummy
			Dummy->TakeDamage(Damage, Character);  // 10 is the damage amount

		}
		Destroy();
	};
}

void Aprojectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile hit its own owner!"));
	}
	else if (OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile hit: %s"), *OtherActor->GetName());
	}

	// Ensure the hit actor is valid and not the same as the projectile itself
	
}

