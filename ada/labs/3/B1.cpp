#include <iostream>
#include <float.h>
#include <stdlib.h>
#include <math.h>
using namespace std;
 
struct Point
{
    int x, y;
};


const int MAX_SIZE = 1e5+10;
const int INF = 1e9;

int compareX(const void* a, const void* b)
{
    Point *p1 = (Point *)a,  *p2 = (Point *)b;
    return (p1->x - p2->x);
}

int compareY(const void* a, const void* b)
{
    Point *p1 = (Point *)a,   *p2 = (Point *)b;
    return (p1->y - p2->y);
}
 
int dist(Point p1, Point p2)
{
    return (p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y);
}
 
int bruteForce(Point P[], int n)
{
    int min = INF;
    if(n == 1)
        return 0;
    for (int i = 0; i < n; ++i)
        for (int j = i+1; j < n; ++j)
            if (dist(P[i], P[j]) < min)
                min = dist(P[i], P[j]);
    return min;
}
 
int min(int x, int y)
{
    return (x < y)? x : y;
}
 
 
int stripClosest(Point strip[], int size, int d)
{
    int min = d;  // Initialize the minimum distance as d
    for (int i = 0; i < size; ++i)
        for (int j = i+1; j < size && (strip[j].y - strip[i].y) < min; ++j)
            if (dist(strip[i],strip[j]) < min)
                min = dist(strip[i], strip[j]);
 
    return min;
}
 
int closestUtil(Point Px[], Point Py[], int n)
{
    if (n <= 3)
        return bruteForce(Px, n);
 
    int mid = n/2;
    Point midPoint = Px[mid];
 
    Point Pyl[mid+10];   
    Point Pyr[n-mid+10];  
    int li = 0, ri = 0;  
    for (int i = 0; i < n; i++)
    {
      if (Py[i].x <= midPoint.x)
         Pyl[li++] = Py[i];
      else
         Pyr[ri++] = Py[i];
    }
 
    int dl = closestUtil(Px, Pyl, mid);
    int dr = closestUtil(Px + mid, Pyr, n-mid);
 
    int d = min(dl, dr);
 
    Point strip[n];
    int j = 0;
    for (int i = 0; i < n; i++)
        if (abs(Py[i].x - midPoint.x) < d)
            strip[j] = Py[i], j++;
 
    return min(d, stripClosest(strip, j, d) );
}
 
int closest(Point P[], int n)
{
    Point Px[n];
    Point Py[n];
    for (int i = 0; i < n; i++)
    {
        Px[i] = P[i];
        Py[i] = P[i];
    }
 
    qsort(Px, n, sizeof(Point), compareX);
    qsort(Py, n, sizeof(Point), compareY);
 
    return closestUtil(Px, Py, n);
}
 
Point P[MAX_SIZE];

int main()
{
    int N;
    cin >> N;

    for(int i=0; i<N; i++)
    {
        cin >> P[i].x >> P[i].y;
        P[i].x = abs(P[i].x);
        P[i].y = abs(P[i].y);
    }
    long long ans = closest(P, N);
    cout << ans << endl;
    return 0;
}