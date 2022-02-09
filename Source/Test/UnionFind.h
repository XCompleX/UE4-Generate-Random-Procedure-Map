// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Set.h"
#include "Containers/Map.h"

/**
 *
 */
class TEST_API UnionFind
{
public:

	int n;
	TArray<int32> parent;
	TArray<int32> size;
	TSet<int32> roots;
	TMap<int32, int32> dictionary;

	UnionFind();

	~UnionFind();

	void make_set(int v);

	int find_set(int v);

	void union_sets(int a, int b);

	void init(int size);
};
