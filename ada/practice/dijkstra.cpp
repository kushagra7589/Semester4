#include <bits/stdc++.h>

using namespace std;

#define ll long long
#define pil pair<int, ll>
#define pli pair<ll, int>
#define F first
#define S second
#define pb push_back
#define mp make_pair

const int MAX_SIZE = 1e5+10;
const ll INF = 1e9+1;
vector< vector< pil > > graph(MAX_SIZE, vector<pil >());
ll distance1[MAX_SIZE];

void dijkstra(int s, int n)
{
	for(int i=1; i<=n; i++)
	{
		distance1[i] = INF;
	}
	distance1[s] = 0;
	set<pli > pq;
	pq.insert(mp(distance1[s], s));
	while(pq.size() > 0)
	{
		auto current = pq.begin()->S;
		pq.erase(pq.begin());
		for(auto i : graph[current])
		{
			int v = i.F;
			int w = i.S;
			if(distance1[v] > distance1[current] + w)
			{
				if(pq.find(mp(distance1[v], v)) != pq.end())
				{
					pq.erase(pq.find(mp(distance1[v], v)));
				}
				distance1[v] = distance1[current] + w;
				pq.insert(mp(distance1[i.F], i.F));
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
		graph[x].pb(mp(y, w));
		graph[y].pb(mp(x, w));
	}
	dijkstra(1, n);
	for(int i=1; i<=n; i++)
	{
		cout << i << " : " << distance1[i] << endl;
	}
	return 0;
}