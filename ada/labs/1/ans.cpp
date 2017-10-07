#include <bits/stdc++.h>

using namespace std;

#define ll long long
#define ld long double
#define F first
#define S second
#define mp make_pair
#define pb push_back

const int MAX_SIZE = 310;
const int INF = 1e9;
int distance1[MAX_SIZE] = {0};
bool visited[MAX_SIZE] = {false};

vector< vector< int > > graph(MAX_SIZE, vector<int>());

void create_edge(int x, int y)
{
	graph[x].pb(y);
}

void bfs(int s)
{
	for(int i=0; i<MAX_SIZE; i++)
	{
		distance1[i] = INF;
		visited[i] = false;
	}
	queue<int> q1;
	q1.push(s);
	visited[s] = true;
	distance1[s] = 0;
	while(!q1.empty())
	{
		int u = q1.front();
		q1.pop();
		for(int i=0; i<graph[u].size(); i++)
		{
			if(visited[graph[u][i]] == false)
			{
				visited[graph[u][i]] = true;
				distance1[graph[u][i]] = distance1[u] + 1;
				q1.push(graph[u][i]);
			}
		}
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	int N, F, K;
	cin >> N >> F >> K;
	while(F--)
	{
		int x, y;
		cin >> x >> y;
		create_edge(x, y);
		create_edge(y, x);
	}
	for(int i=1; i<=N; i++)
	{
		bfs(i);
		int count = 0;
		for(int j = 1; j<=N; j++)
		{
			if(distance1[j] <= K && distance1[j] > 1)
				count += 1;
		}
		cout << count << endl;
	}
	return 0;
}