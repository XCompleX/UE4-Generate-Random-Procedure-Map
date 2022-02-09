// Fill out your copyright notice in the Description page of Project Settings.


#include "Triangulating.h"

// Sets default values for this component's properties
UTriangulating::UTriangulating()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTriangulating::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTriangulating::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<TArray<double>> UTriangulating::big_triangle(TArray<TArray<double>> points) {
    float minx = 1000000, maxx = -1000000, miny = 1000000, maxy = -1000000;
    for (int i = 0; i < points.Num(); i++) {
        minx = minx < points[i][0] ? minx : points[i][0];
        miny = miny < points[i][1] ? miny : points[i][1];
        maxx = maxx > points[i][0] ? maxx : points[i][0];
        maxy = maxy > points[i][1] ? maxy : points[i][1];
    }
    float dx = maxx - minx, dy = maxy - miny;
    float dxy = dx > dy ? dx : dy;
    float midx = dx * 0.5 + minx, midy = dy * 0.5 + miny;

    return { {midx - 10 * dxy,midy - 10 * dxy},{midx,midy + 10 * dxy},{midx + 10 * dxy,midy - 10 * dxy} };
}

TArray<double> UTriangulating::circumcircle_of_triangle(TArray<TArray<double>> points, int v1, int v2, int v3) {
    float x1 = points[v1][0], y1 = points[v1][1];
    float x2 = points[v2][0], y2 = points[v2][1];
    float x3 = points[v3][0], y3 = points[v3][1];
    float dy12 = abs(y1 - y2), dy23 = abs(y2 - y3);
    float xc, yc;

    if (dy12 < EPS) {
        float m2 = -((x3 - x2) / (y3 - y2));
        float mx2 = (x2 + x3) / 2, my2 = (y2 + y3) / 2;

        xc = (x1 + x2) / 2;
        yc = m2 * (xc - mx2) + my2;
    }
    else if (dy23 < EPS) {
        float m1 = -((x2 - x1) / (y2 - y1));
        float mx1 = (x1 + x2) / 2, my1 = (y1 + y2) / 2;

        xc = (x2 + x3) / 2;
        yc = m1 * (xc - mx1) + my1;
    }
    else {
        float m1 = -((x2 - x1) / (y2 - y1)), m2 = -((x3 - x2) / (y3 - y2));
        float mx1 = (x1 + x2) / 2, my1 = (y1 + y2) / 2;
        float mx2 = (x2 + x3) / 2, my2 = (y2 + y3) / 2;

        xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);

        if (dy12 > dy23)
            yc = m1 * (xc - mx1) + my1;
        else
            yc = m2 * (xc - mx2) + my2;
    }

    float dx = x2 - xc, dy = y2 - yc;

    return { float(v1), float(v2), float(v3), xc, yc, dx * dx + dy * dy };
}

void UTriangulating::delete_multiples_edges(TArray<double>& edges) {
    for (int j = edges.Num() - 1; j >= 0;) {
        float b = -10000000, a = -10000000;
        if (j >= 0 && j < edges.Num())
            b = edges[j]; j--;
        if (j >= 0 && j < edges.Num())
            a = edges[j]; j--;
        float n = -10000000, m = -10000000;
        for (int i = j; i >= 0;) {
            if (i >= 0 && i < edges.Num())
                n = edges[i]; i--;
            if (i >= 0 && i < edges.Num())
                m = edges[i]; i--;

            if (a == -10000000 || b == -10000000 || n == -10000000 || m == -10000000)
                break;

            if (a == m && b == n) {
                edges.RemoveAt(1 + j, 2);
                edges.RemoveAt(1 + j, 2);
                break;
            }
            if (a == n && b == m) {
                edges.RemoveAt(1 + j, 2);
                edges.RemoveAt(1 + i, 2);
                break;
            }
        }
    }
}

void UTriangulating::triangulate(TArray<int32> vertices, TArray<int32> groundPoints, TArray<int32>& triangles) {
    TArray<TArray<double>> pts;
    TArray<TArray<double>> pts2;

    for (int i = 0; i < vertices.Num(); i += 2) {
        pts.Add({ double(vertices[i]), double(vertices[i + 1]) });
        pts2.Add({ double(vertices[i]), double(vertices[i + 1]) });
    }

    int n = pts.Num();

    if (n < 3) return;

    TArray<int32> ind;

    for (int i = 0; i < n; i++)
        ind.Add(i);

    for (int i = 0; i < pts.Num() - 1; i++) {
        if (pts2[i][0] < pts2[i + 1][0]) {
            int b = pts2[i][0];
            pts2[i][0] = pts2[i + 1][0];
            pts2[i + 1][0] = b;

            b = ind[i];
            ind[i] = ind[i + 1];
            ind[i + 1] = b;

            i = -1;
        }
    }

    TArray<TArray<double>> big = big_triangle(pts);
    pts.Add(big[0]);
    pts.Add(big[1]);
    pts.Add(big[2]);

    TArray<TArray<double>> cur_points = { circumcircle_of_triangle(pts, n, n + 1, n + 2) };
    TArray<TArray<double>> ans;
    TArray<double> edges;

    for (int i = ind.Num() - 1; i >= 0; i--) {
        for (int j = cur_points.Num() - 1; j >= 0; j--) {
            float dx = pts[ind[i]][0] - cur_points[j][3];
            if (dx > 0 && dx * dx > cur_points[j][5]) {
                ans.Add(cur_points[j]);

                cur_points.RemoveAt(j, 1);

                continue;
            }

            float dy = pts[ind[i]][1] - cur_points[j][4];
            if (dx * dx + dy * dy - cur_points[j][5] > EPS) {
                continue;
            }

            edges.Add(cur_points[j][0]);
            edges.Add(cur_points[j][1]);
            edges.Add(cur_points[j][1]);
            edges.Add(cur_points[j][2]);
            edges.Add(cur_points[j][2]);
            edges.Add(cur_points[j][0]);

            cur_points.RemoveAt(j, 1);
        }

        delete_multiples_edges(edges);

        for (int j = edges.Num() - 1; j >= 0;) {
            float b = edges[j]; j--;
            if (j < 0) break;
            float a = edges[j]; j--;
            cur_points.Add(circumcircle_of_triangle(pts, a, b, ind[i]));
        }
        edges = {};
    }

    for (int i = cur_points.Num() - 1; i >= 0; i--) {
        ans.Add(cur_points[i]);
    }

    for (int i = 0; i < ans.Num(); i++) {
        if (ans[i][0] < n && ans[i][1] < n && ans[i][2] < n) {
            triangles.Add(ans[i][0]);
            triangles.Add(ans[i][1]);
            triangles.Add(ans[i][2]);
        }
    }

    for (int i = 0; i < triangles.Num(); i += 3) {
        int xa = pts[triangles[i]][0]; int ya = pts[triangles[i]][1];
        int xb = pts[triangles[i + 1]][0]; int yb = pts[triangles[i + 1]][1];
        int xc = pts[triangles[i + 2]][0]; int yc = pts[triangles[i + 2]][1];
        bool check = false;
        for (int j = 0; j < groundPoints.Num(); j += 2) {
            int xd = groundPoints[j];
            int yd = groundPoints[j + 1];
            if ((((xd - xa) * (yb - ya) - (yd - ya) * (xb - xa)) * ((xc - xa) * (yb - ya) - (yc - ya) * (xb - xa)) >= 0) &&
                (((xd - xb) * (yc - yb) - (yd - yb) * (xc - xb)) * ((xa - xb) * (yc - yb) - (ya - yb) * (xc - xb)) >= 0) &&
                (((xd - xc) * (ya - yc) - (yd - yc) * (xa - xc)) * ((xb - xc) * (ya - yc) - (yb - yc) * (xa - xc)) >= 0)) {
                check = true;
                break;
            }
        }
        if (!check) {
            triangles.RemoveAt(i, 3);
            i -= 3;
        }
    }

    /*FString file1 = "C:\\Users\\vadim\\Documents\\Unreal Projects\\Test\\Triangles.txt";

    IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

    FString Triangles;

    Triangles.AppendChar('[');
    for (int i = 0; i < triangles.Num(); i++) {
        Triangles.AppendInt(triangles[i]);
        Triangles.AppendChar(',');
    }
    Triangles.AppendChar(']');

    if (FileManager.FileExists(*file1))
        FFileHelper::SaveStringToFile(Triangles, *file1);*/
}
