// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class TEST_API BinaryMap
{
public:
	int32 n;
	TArray<int32> inner;

	BinaryMap();

	~BinaryMap();

	TArray<int32> get_orthogonal_neighbor_indices(int tile_index);

	TArray<int32> get_diagonal_neighbor_indices(int tile_index);

	bool is_water(int tile_index);

	bool is_land(int tile_index);

	bool is_inner_land(int tile_index);

	bool is_border_land(int tile_index);

	void init(TArray<int32> map);
};
