// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Triangulating.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEST_API UTriangulating : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTriangulating();

	double EPS = 1e-7;

	TArray<TArray<double>> big_triangle(TArray<TArray<double>> points);
	TArray<double> circumcircle_of_triangle(TArray<TArray<double>> points, int v1, int v2, int v3);
	void delete_multiples_edges(TArray<double>& edges);
	UFUNCTION(BlueprintCallable, Category = "Triangulating")
		void triangulate(TArray<int32> vertices, TArray<int32> groundPoints, TArray<int32>& triangles);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
