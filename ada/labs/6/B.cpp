#include <bits/stdc++.h>

using namespace std;

#define ll long long
#define ld long double
#define pll pair<ll, ll>
#define mp make_pair
#define pb push_back
#define F first
#define S second

const int MAX_SIZE = 1e4+10;

vector< ll > graph[MAX_SIZE];

bool visited[MAX_SIZE] = {false};

pair<ll, ll> sum_till_now[MAX_SIZE];

int special[MAX_SIZE];

int parent[MAX_SIZE];

ll weight[MAX_SIZE];

void dfs(int i, int j)
{
	visited[i] = true;
	// cout << "Currently at : " << i << " " << j << endl;
	// for(int x=0; x<=j; x++)
	// {
	// 	// cout << sum_till_now[x].F << " " ;
	// }
	// cout << endl;
	// ll check = 0.5 * 
	// while(l < u)
	// {
	// 	mid = (l+u)/2;
	// 	// cout << "l = " << l << " u = "  << u << endl;
	// 	// cout << "checking " << sum_till_now[mid].F << endl;
	// 	if(sum_till_now[mid].F < 0.5 * sum_till_now[j].F){
	// 		// cout << " con 1" << endl;
	// 		l = mid + 1;
	// 	}
	// 	else{
	// 		// cout << "cond 2" << endl;
	// 		u = mid;
	// 	}
	// }
	// special[i] = sum_till_now[l].S;
	// sum_till_now[j+1].F = sum_till_now[j].F + weight[i];
	// sum_till_now[j+1].S = i;
	for(auto &child : graph[i])
	{
		if(visited[child] == false)
		{
			sum_till_now[child] = sum_till_now[parent[child]] + weight[child]; 
			parent[child] = i;
			for(int start = special[parent[child]]; start <= parent[child]; start++)
			{
				if(sum_till_now[start] >= sum_till_now[parent[child]])
					special[child] = start;
			}
			{
				if()
			}
			dfs(child, j+1);
		}
	}	
}

ll ans[MAX_SIZE];

void dp(int i)
{
	// cout << "i = " << i << endl;
	visited[i] = true;
	for(auto &child : graph[i])
	{
		if(visited[child] == false)
		{
			// cout << child << ": " << ans[i] + weight[i] << " " << ans[special[child]] + weight[special[child]] << endl;
			ans[child] = min(ans[i] + weight[i], ans[special[child]] + weight[special[child]]);
			dp(child);
		}
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	int t;
	int n;
	cin >> t;
	while(t--)
	{
		memset(visited, false, sizeof(visited));
		cin >> n;
		// cin >> x >> y;
		int x, y;
		for(int i=1; i<n; i++)
		{
			cin >> x >> y;
			graph[x].pb(y);
			graph[y].pb(x);
		}
		for(int i=1; i<=n; i++)
		{
			cin >> weight[i];
		}
		special[1] = 0;
		sum_till_now[0].F = 0;
		sum_till_now[0].S = 0;
		dfs(1, 0);
		// for(int i=1; i<=n; i++)
		// {
		// 	cout << i << " : " << special[i] << " : " << parent[i] << endl;
		// }
		ans[1] = 0;
		ans[0] = 0;
		weight[0] = 0;
		memset(visited, false, sizeof(visited));
		for(int i=0; i<=n; i++)
			visited[i] = false;
		dp(1);
		for(int i=1; i<=n; i++)
		{
			cout << ans[i] << " ";
		}
		cout << endl;
	}	
	return 0;
}