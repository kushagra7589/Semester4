#include <bits/stdc++.h>

using namespace std;

#define ll long long
#define pb push_back
#define pil pair<int, ll>
#define mp make_pair
#define F first
#define S second

const int MAX_SIZE = 1e3+10;

vector< vector< pil > > graph(MAX_SIZE, vector< pil >());
vector< ll > paths;
bool visited[MAX_SIZE];
multiset< ll, greater<ll> > max_edge;
ll path_dist = 0;

void dfs(int src, int dest)
{
	visited[src] = true;
	// cout << "currently at " << src << " path_dist = " << path_dist << endl;
	for(auto i : graph[src])
	{
		int u = i.F;
		ll w = i.S;
		if(visited[u] == false)
		{
			path_dist += w;
			max_edge.insert(w);
			if(u != dest)
			{
				dfs(u, dest);
			}
			else
			{
				paths.pb(path_dist - *max_edge.begin());		
			}
			path_dist -= w;
			max_edge.erase(max_edge.find(w));
		}
	}
	visited[src] = false;
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
	int src, dest;
	cin >> src >> dest;
	path_dist = 0;
	dfs(src, dest);
	ll min_dist = 1e9;
	for(auto i : paths)
	{
		min_dist = min(min_dist, i);
	}
	cout << min_dist << endl;
	return 0;
}