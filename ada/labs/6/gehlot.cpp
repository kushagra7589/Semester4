#include<bits/stdc++.h>

using namespace std;
vector<int> V[10005];
int64_t w[10005];
int64_t ans[10005];
bool visit[10005];
vector<int> q;


void dfs(int node,int k,int64_t w_before,int64_t kw_before,int parent,int index)
{
	if(node==1)
	{
		ans[node]=0;
		kw_before=w[1];
	}
	else
	{
		ans[node]=ans[parent]+w[parent];
		if((kw_before*2)>=w_before)
		{
			ans[node]=min(ans[node],ans[k]+w[k]);
		}
		else
		{
			int newnode;
			index++;
			for(;index<q.size();++index)
			{
				newnode=q[index];
				kw_before+=w[newnode];
				if((kw_before*2)>=w_before)
				{
					k=newnode;
					break;
				}
			}
			ans[node]=min(ans[node],ans[k]+w[k]);
		}
	}
	visit[node]=true;
	if(node!=1){q.push_back(node);}
	for(int i=0;i<V[node].size();i++)
	{
		if(!visit[V[node][i]])
		{
			dfs(V[node][i],k,w_before+w[node],kw_before,node,index);
		}
	}
	//cout<<node<<" "<<parent<<" "<<k<<"\n";
	q.pop_back();
}

void solve(int n)
{
	q.push_back(1);
	dfs(1,1,0,0,1,0);
}

int main()
{
	int i,n,a,b,t;
	cin>>t;
	while(t--)
	{
		cin>>n;
		q.clear();
		for(i=0;i<=n;i++){V[i].clear();}
		for(i=0;i<n-1;i++)
		{
			cin>>a>>b;
			V[a].push_back(b);
			V[b].push_back(a);
		}
		for(i=1;i<=n;i++)
		{
			visit[i]=false;
			cin>>w[i];
		}
		solve(n);
		for(i=1;i<=n;i++)
		{
			cout<<ans[i]<<" ";
		}
		cout<<"\n";
	}
	return 0;
}
