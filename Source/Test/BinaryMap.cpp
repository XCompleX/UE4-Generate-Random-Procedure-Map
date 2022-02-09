// Fill out your copyright notice in the Description page of Project Settings.


#include "BinaryMap.h"

BinaryMap::BinaryMap()
{
}

BinaryMap::~BinaryMap()
{
}

void BinaryMap::init(TArray<int32> map) {
    this->n = (int32)sqrt(map.Num());
    this->inner = map;
}

TArray<int32> BinaryMap::get_orthogonal_neighbor_indices(int tile_index) {
    int i = tile_index / n;
    int j = tile_index % n;

    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: n: %i"), i));

    TArray<int32> orthogonal_neighbors;
    orthogonal_neighbors.Add((i - 1) * n + j);
    orthogonal_neighbors.Add((i + 1) * n + j);
    orthogonal_neighbors.Add(i * n + (j - 1));
    orthogonal_neighbors.Add(i * n + (j + 1));

    return orthogonal_neighbors;
}

TArray<int32> BinaryMap::get_diagonal_neighbor_indices(int tile_index) {
    int i = tile_index / n;
    int j = tile_index % n;

    TArray<int32> diagonal_neighbors;

    diagonal_neighbors.Add((i - 1) * n + (j - 1));
    diagonal_neighbors.Add((i - 1) * n + (j + 1));
    diagonal_neighbors.Add((i + 1) * n + (j - 1));
    diagonal_neighbors.Add((i + 1) * n + (j + 1));

    return diagonal_neighbors;
}

bool BinaryMap::is_water(int tile_index) {
    return inner[tile_index] == 0;
}

bool BinaryMap::is_land(int tile_index) {
    return !is_water(tile_index);
}

bool BinaryMap::is_inner_land(int tile_index) {
    if (is_water(tile_index)) {
        return false;
    }

    TArray<int32> orthogonal_neighbor_indices = get_orthogonal_neighbor_indices(tile_index);
    
    for (int neighbor_index : orthogonal_neighbor_indices) {
        if (is_water(neighbor_index)) {
            return false;
        }
    }

    return true;
}

bool BinaryMap::is_border_land(int tile_index) {
    return is_land(tile_index) && !is_inner_land(tile_index);
}
