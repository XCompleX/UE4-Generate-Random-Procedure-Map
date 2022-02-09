// Fill out your copyright notice in the Description page of Project Settings.


#include "UnionFind.h"



UnionFind::UnionFind()
{
}

UnionFind::~UnionFind()
{
}

void UnionFind::init(int x)
{
    this->n = x;
    this->parent.SetNum(x);
    this->size.SetNum(x);
}

void UnionFind::make_set(int v) {
    parent[v] = v;
    size[v] = 1;
    roots.Add(v);
}

int UnionFind::find_set(int v) {
    if (parent[v] == v) return v;
    return parent[v] = find_set(parent[v]);
}

void UnionFind::union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);

    if (a == b) return;
    if (size[a] < size[b])
        Swap(a, b);

    parent[b] = a;
    size[a] += size[b];
    roots.Remove(b);
}
