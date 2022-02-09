// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnionFind.h"
#include "BinaryMap.h"
#include "Point.h"
#include "Containers/Set.h"
#include "Containers/Map.h"
#include "FindIsland.generated.h"

USTRUCT(BlueprintType)
struct FinnerArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FLOL")
		TArray<int32> innerArray;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEST_API UFindIsland : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UnionFind uf;
	BinaryMap map;
	int32 size;

	UFindIsland();

	UFUNCTION(BlueprintCallable, Category="FindIsland")
		void init(TArray<int32> mapArray);
	UFUNCTION(BlueprintCallable, Category = "FindIsland")
		void find_border_islands(TArray<int32>& BordersIslandsMap /*, TArray<int32>& BordersIslandsMapLowVerteces*/);
	UFUNCTION(BlueprintCallable, Category = "FindIsland")
		void find_islands(TArray<int32>& islandsMap);
	UFUNCTION(BlueprintCallable, Category = "FindIsland")
		void find_border_islands_width_lakes(TArray<int32>& BordersIslandsMap, /*TArray<int32>& BordersIslandsMapLowerVerteces,*/ TArray<FinnerArray>& verteces, TArray<FinnerArray>& groundVerteces);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};