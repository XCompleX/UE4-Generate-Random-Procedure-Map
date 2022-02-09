// Fill out your copyright notice in the Description page of Project Settings.


#include "FindIsland.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UFindIsland::UFindIsland()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void UFindIsland::BeginPlay()
{
    Super::BeginPlay();

    // ...

}


// Called every frame
void UFindIsland::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}


void UFindIsland::init(TArray<int32> mapArray) {
    this->size = (int32)sqrt(mapArray.Num());
    this->map.init(mapArray);
    this->uf.init(mapArray.Num());
}

void UFindIsland::find_islands(TArray<int32>& islandsMap) {
    for (int i = 0; i < size * size; i++) {
        uf.make_set(i);
    }

    int water_tile_index = -1;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int current_index = i * size + j;

            if (map.is_land(current_index)) {
                TArray<int32> orthogonal_neighbor_indices = map.get_orthogonal_neighbor_indices(current_index);

                for (int neighbor_index : orthogonal_neighbor_indices) {
                    if (map.is_land(neighbor_index)) {
                        uf.union_sets(current_index, neighbor_index);
                    }
                }
            }
            else {
                if (water_tile_index == -1) {
                    water_tile_index = current_index;
                }
                else {
                    uf.union_sets(water_tile_index, current_index);
                }
            }
        }
    }

    for (int i = 0, j = 2; i < uf.roots.Num(); i++) {
        if (uf.roots.Array()[i] == water_tile_index)
            uf.dictionary.Add(uf.roots.Array()[i], 0);
        else
            uf.dictionary.Add(uf.roots.Array()[i], j++);
    }


    for (int i = 0; i < uf.parent.Num(); i++)
        islandsMap.Add(uf.dictionary[uf.find_set(i)]);
}

void UFindIsland::find_border_islands(TArray<int32>& BordersIslandsMap /*, TArray<int32>& BordersIslandsMapLowVerteces*/) {
    for (int i = 0; i < size * size; i++) {
        uf.make_set(i);
    }
    
    int water_tile_index = -1;
    int inner_land_tile_index = -1;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int current_index = i * size + j;

            if (map.is_border_land(current_index)) {
                TArray<int32> orthogonal_neighbor_indices = map.get_orthogonal_neighbor_indices(current_index);
                TArray<int32> diagonal_neighbor_indices = map.get_diagonal_neighbor_indices(current_index);


                for (int neighbor_index : orthogonal_neighbor_indices) {
                    if (map.is_border_land(neighbor_index)) {
                        uf.union_sets(current_index, neighbor_index);
                    }
                }

                for (int neighbor_index : diagonal_neighbor_indices) {
                    if (map.is_border_land(neighbor_index)) {
                        uf.union_sets(current_index, neighbor_index);
                    }
                }
            }
            else if (map.is_land(current_index)) {
                if (inner_land_tile_index == -1) {
                    inner_land_tile_index = current_index;
                }
                else {
                    uf.union_sets(inner_land_tile_index, current_index);
                }
            }
            else {
                if (water_tile_index == -1) {
                    water_tile_index = current_index;
                }
                else {
                    uf.union_sets(water_tile_index, current_index);
                }
            }
        }
    }

    for (int i = 0, j = 2; i < uf.roots.Num(); i++) {
        if (uf.roots.Array()[i] == water_tile_index)
            uf.dictionary.Add(uf.roots.Array()[i], 0);
        else if (uf.roots.Array()[i] == inner_land_tile_index)
            uf.dictionary.Add(uf.roots.Array()[i], 1);
        else
            uf.dictionary.Add(uf.roots.Array()[i], j++);
    }

    for (int i = 0; i < uf.parent.Num(); i++) {
        BordersIslandsMap.Add(uf.dictionary[uf.find_set(i)]);
        //BordersIslandsMapLowVerteces.Add(uf.dictionary[uf.find_set(i)]);
    }

    /*TArray<int32> delVerteces;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int current_index = i * size + j;
            if (BordersIslandsMapLowVerteces[current_index] != 0 && BordersIslandsMapLowVerteces[current_index] != 1) {
                TArray<int32> orthogonal_neighbor_indices = map.get_orthogonal_neighbor_indices(current_index);
                TArray<int32> diagonal_neighbor_indices = map.get_diagonal_neighbor_indices(current_index);
                
                if (BordersIslandsMapLowVerteces[orthogonal_neighbor_indices[0]] == BordersIslandsMapLowVerteces[orthogonal_neighbor_indices[1]] && BordersIslandsMapLowVerteces[orthogonal_neighbor_indices[1]] == BordersIslandsMapLowVerteces[current_index])
                    delVerteces.Add(current_index);
                if (BordersIslandsMapLowVerteces[orthogonal_neighbor_indices[2]] == BordersIslandsMapLowVerteces[orthogonal_neighbor_indices[3]] && BordersIslandsMapLowVerteces[orthogonal_neighbor_indices[3]] == BordersIslandsMapLowVerteces[current_index])
                    delVerteces.Add(current_index);
                if (BordersIslandsMapLowVerteces[diagonal_neighbor_indices[0]] == BordersIslandsMapLowVerteces[diagonal_neighbor_indices[3]] && BordersIslandsMapLowVerteces[diagonal_neighbor_indices[3]] == BordersIslandsMapLowVerteces[current_index])
                    delVerteces.Add(current_index);
                if (BordersIslandsMapLowVerteces[diagonal_neighbor_indices[1]] == BordersIslandsMapLowVerteces[diagonal_neighbor_indices[2]] && BordersIslandsMapLowVerteces[diagonal_neighbor_indices[2]] == BordersIslandsMapLowVerteces[current_index])
                    delVerteces.Add(current_index);
            }
        }
    }
    for (int i: delVerteces)
        BordersIslandsMapLowVerteces[i] = 1;*/
}

void UFindIsland::find_border_islands_width_lakes(TArray<int32>& BordersIslandsMap, /*TArray<int32>& BordersIslandsMapLowerVerteces,*/ TArray<FinnerArray>& verteces, TArray<FinnerArray>& groundVerteces) {
    TArray<int32> IslandsMap;

    find_islands(IslandsMap);
    find_border_islands(BordersIslandsMap);
    for (int i = 0; i < IslandsMap.Num(); i++) {
        if (BordersIslandsMap[i] != 0 && BordersIslandsMap[i] != 1) {
            BordersIslandsMap[i] = IslandsMap[i];
        }
    }

    TSet<int32> islandsNum;
    for (int num : BordersIslandsMap)
        if(num != 0 && num != 1)
            islandsNum.Add(num);

    for (int num : islandsNum) {
        FinnerArray innerArrayVerteces;
        FinnerArray innerArrayGroundVerteces;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                int current_index = i * size + j;
                if (BordersIslandsMap[current_index] == num) {
                    innerArrayVerteces.innerArray.Add(i);
                    innerArrayVerteces.innerArray.Add(j);
                }
                if (IslandsMap[current_index] == num && BordersIslandsMap[current_index] == 1) {
                    innerArrayGroundVerteces.innerArray.Add(i);
                    innerArrayGroundVerteces.innerArray.Add(j);
                }
            }
        }
        verteces.Add(innerArrayVerteces);
        groundVerteces.Add(innerArrayGroundVerteces);
    }
    /*
    FString file1 = "C:\\Users\\vadim\\Documents\\Unreal Projects\\Test\\Border.txt";
    FString file2 = "C:\\Users\\vadim\\Documents\\Unreal Projects\\Test\\Ground.txt";

    IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

    FString Verteces;
    FString Grounds;

    Grounds.AppendChar('[');
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int current_index = i * size + j;
            if (BordersIslandsMap[current_index] == 1) {
                Grounds.AppendChar('[');
                groundsVerteces.Add(i);
                Grounds.AppendInt(i);
                Grounds.AppendChar(',');
                Grounds.AppendInt(j);
                groundsVerteces.Add(j);
                Grounds.AppendChar(']');
                Grounds.AppendChar(',');
            }
        }
    }
    Grounds.AppendChar(']');
    
    Verteces.AppendChar('[');
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int current_index = i * size + j;
            if (BordersIslandsMap[current_index] == 2) {
                Verteces.AppendChar('[');
                verteces.Add(i);
                Verteces.AppendInt(i);
                Verteces.AppendChar(',');
                Verteces.AppendInt(j);
                verteces.Add(j);
                Verteces.AppendChar(']');
                Verteces.AppendChar(',');
            }
        }
    }
    Verteces.AppendChar(']');

    if (FileManager.FileExists(*file1))
        FFileHelper::SaveStringToFile(Verteces, *file1);
    if (FileManager.FileExists(*file2))
        FFileHelper::SaveStringToFile(Grounds, *file2);
        */
}