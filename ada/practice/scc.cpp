#include <bits/stdc++.h>

using namespace std;

#define mp make_pair
#define pb push_back
#define F first
#define S second

const int MAX_SIZE = 1e5+10;

vector< vector< int > > graph(MAX_SIZE, vector<int>());
vector< vector< int > > tran(MAX_SIZE, vector<int>());
stack<int> S1;
bool visited[MAX_SIZE];

void dfsStack(int s)
{
	visited[s] = true;
	for(auto i : graph[s])
	{
		if(visited[i] == false)
			dfsStack(i);
	}
	S1.push(s);
}

void dfs(int s)
{
	visited[s] = true;
	for(auto i : tran[s])
	{
		if(visited[i] == false)
		{
			dfs(i);
		}
	}
	cout << s << " ";
}

int main()
{
	int n, m;
	cin >> n >> m;
	int x, y;
	for(int i=0; i<m; i++)
	{
		cin >> x >> y;
		graph[x].pb(y);
		tran[y].pb(x);
	}
	for(int i=0; i<n; i++)
	{
		visited[i] = false;
	}
	for(int i=0; i<n; i++)
	{
		if(visited[i] == false)
		{
			dfsStack(i);
		}
	}
	for(int i=0; i<n; i++)
	{
		visited[i] = false;
	}
	while(!S1.empty())
	{
		int i = S1.top();
		S1.pop();
		// cout << i << " ";
		if(visited[i] == false)
		{
			dfs(i);
			cout << endl;
		}
	}
	return 0;
}