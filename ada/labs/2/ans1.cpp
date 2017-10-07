#include <bits/stdc++.h>

using namespace std;

#define ll long long
#define pb push_back
#define mp make_pair
#define F first	
#define S second
#define pil pair<int, ll>
#define pli pair<ll, int>

const int MAX_SIZE = 3e3+10;
const ll INF = 1e10+1;
vector< set< pil > > graph(MAX_SIZE, set< pil >());
ll Adist[MAX_SIZE];
ll BDist[MAX_SIZE];
ll CDist[MAX_SIZE];

ll minA[MAX_SIZE];
ll minB[MAX_SIZE];
ll minC[MAX_SIZE];

void shortestPath(int src, int n, ll dist[])
{
	for(int i=1; i<=n;i++)
	{
		dist[i] = INF;
	}
	dist[src] = 0;
	set< pli > pq;
	pq.insert(mp(dist[src], src));
	while(pq.size() > 0)
	{
		auto u = (*pq.begin()).S;
		pq.erase(pq.begin());
		for(auto i : graph[u])
		{
			auto v = i.F;
			auto w = i.S;
			if(dist[v] > dist[u] + w)
			{
				if(pq.find(mp(dist[v], v)) != pq.end())
					pq.erase(mp(dist[v], v));
				dist[v] = dist[u] + w;
				pq.insert(mp(dist[v], v));
			}
		}
	}
}

int arr[MAX_SIZE];

int make_sets()
{
	int a;
	cin >> a;
	for(int i = 0; i<a; i++)
	{
		cin >> arr[i];
	}
	// for(int i=0; i<a; i++)
	// {
	// 	for(int j=0; j<a; j++)
	// 	{
	// 		if(i != j)
	// 		{
	// 			graph[i].insert(mp(j, 0));
	// 			graph[j].insert(mp(i, 0));
	// 		}
	// 	}
	// }
	return a;
}

void unmake_sets(int a)
{
	for(int i=0; i<a; i++)
	{
		for(int j=0; j<a; j++)
		{
			if(i != j)
			{
				graph[i].erase(mp(j, 0));
				graph[j].erase(mp(i, 0));
			}
		}
	}
}

int main()
{
	int n, m;
	cin >> n >> m;
	int x, y;
	ll w;
	for(int i=0; i<m; i++)
	{
		cin >> x >> y >> w;
		graph[x].insert(mp(y, w));
		graph[y].insert(mp(x, w));
	}
	int a = make_sets();
	for(int i=1; i<=n; i++)
		minA[i] = INF;
	for(int i=0; i<a; i++)
	{
		shortestPath(arr[i], n, Adist);
		for(int i=1; i<=n; i++)
		{
			minA[i] = min(minA[i], Adist[i]);
		}
	}
	// shortestPath(X, n, Adist);
	// unmake_sets(X);
	for(int i=1; i<=n; i++)
		minB[i] = INF;
	int b = make_sets();
	for(int i=0; i<b; i++)
	{
		shortestPath(arr[i], n, BDist);
		for(int i=1; i<=n; i++)
		{
			minB[i] = min(minB[i], BDist[i]);
		}
	}
	// shortestPath(Y, n, BDist);
	// unmake_sets(Y);
	int c = make_sets();
	for(int i=1; i<=n; i++)
		minC[i] = INF;
	for(int i=0; i<c; i++)
	{
		shortestPath(arr[i], n, CDist);
		for(int i=1; i<=n; i++)
		{
			minC[i] = min(minC[i], CDist[i]);
		}
	}
	// for(int i=0; i<a; i++)
	// {
	// 	shortestPath(arr[i], n, Adist);
	// 	for(int i=1; i<=n; i++)
	// 	{
	// 		minA[i] = min(minA[i], Adist[i]);
	// 	}
	// }
	// shortestPath(Z, n, CDist);
	ll ans = INF;
	for(int i=1; i<=n; i++)
	{
		ans = min(ans, minA[i] + minB[i] + minC[i]);
	}
	cout << ans << endl;
	return 0;
}