#include<bits/stdc++.h>

using namespace std;

#define ld long double

const int MAX_SIZE = 105;

ld graph[MAX_SIZE][MAX_SIZE];

ld dis[MAX_SIZE][MAX_SIZE];

int main()
{
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--)
	{
		int n;
		cin >> n;
		for(int i=1; i<=n; i++)
		{
			for(int j=1; j<=n; j++)
			{
				cin >> graph[i][j];
				graph[i][j] = log(1.0/graph[i][j]);
			}
		}

		for(int k=1; k<=n; k++)
		{
			for(int i=1; i<=n; i++)
			{
				for(int j=1; j<=n; j++)
				{
					if(graph[i][j] > graph[i][k] + graph[k][j])
						graph[i][j] = graph[i][k] + graph[k][j];
				}
			}
		}
		int flag = 0;
		int i;
		for(i=1; i<=n; i++)
		{
			if(graph[i][i] < 0)
			{
				cout << "YES" << endl;
				break;
			}
		}
		if(i == n+1)
		{
			cout << "NO" << endl;
		}
	}
	return 0;
}