#pragma warning(disable:4786)
//#pragma comment(linker, "/STACK:16777216")
#pragma comment(linker, "/STACK:1073741824")
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<vector>
#include<set>
#include<map>
#include<functional>
#include<string>
#include<cstring>
#include<cstdlib>
#include<queue>
#include<utility>
#include<fstream>
#include<sstream>
#include<cmath>
#include<stack>
#include<assert.h>
#include<complex>
#include<bitset>
using namespace std;

#define MEM(a, b) memset(a, (b), sizeof(a))
#define CLR(a) memset(a, 0, sizeof(a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(X) ( (X) > 0 ? (X) : ( -(X) ) )
#define S(X) ( (X) * (X) )
#define SZ(V) (int )V.size()
#define FORN(i, n) for(i = 0; i < n; i++)
#define FORAB(i, a, b) for(i = a; i <= b; i++)
#define ALL(V) V.begin(), V.end()

typedef pair<int,int> PII;
typedef pair<double, double> PDD;
typedef vector<int> VI;

#define IN(A, B, C) assert( B <= A && A <= C)

//typedef int LL;
//typedef __int64 LL;

vector<int> Prime;
int mark[50002];

void sieve(int n)
{
	int i,j,limit=sqrt(n*1.)+2;

	mark[1] = 1;
	for(i=4;i<=n;i+=2) mark[i]=1;

	Prime.push_back(2);
	for(i=3;i<=n;i+=2)
		if(!mark[i])
		{
			Prime.push_back(i);

			if(i<=limit)
			{
				for(j=i*i;j<=n;j+=i*2)
				{
					mark[j] = 1;
				}
			}
		}
}


VI V[200003];
vector<PII> E[200003];
int n, ans, dummy;
vector<int>::iterator iV;

int xx;

void Binarize(int at, int parent)
{
	int x, y, sz, i;

	sz = V[at].size();
// 	printf("%d %d\n", xx++, sz);

	if(parent)
	{
		E[at].push_back(PII(parent, at<=n));
	}

	if(sz <= 2)
	{
		for(i = 0; i < sz; i++)
		{
			if(at <= n)
				V[V[at][i]].erase( find(V[V[at][i]].begin(), V[V[at][i]].end(), at) );

			E[at].push_back(PII(V[at][i], 1));
			Binarize(V[at][i], at);
		}
	}
	else
	{
		x = dummy + 1;
		y = dummy + 2;
		dummy += 2;

		for(i = 0; i < sz; i++)
		{
			if(i < sz/2) V[x].push_back(V[at][i]);
			else		 V[y].push_back(V[at][i]);

			if(at <= n)
				V[V[at][i]].erase( find(V[V[at][i]].begin(), V[V[at][i]].end(), at) );
		}

		E[at].push_back( PII(x, 0) );
		E[at].push_back( PII(y, 0) );

		Binarize(x, at);
		Binarize(y, at);
	}
}

int subtree[200005], best_pos, best_value, best_d;
int parent[200005], _sz;

int DFS(int at, int parent)
{
	subtree[at] = 1;
	::parent[at] = parent;

	int here, sz = E[at].size(), i;
	for(i = 0; i < sz; i++)
	{
		if(E[at][i].first == parent) 
		{
			here = E[at][i].second;
			continue;
		}

		subtree[at] += DFS(E[at][i].first, at);
	}

	if(parent && (best_pos == -1 || best_value > ABS(_sz/2 - subtree[at])))
	{
		best_pos = at, best_value = ABS(_sz/2 - subtree[at]), best_d = here;
	}

	return subtree[at];
}

void populate_depth(int at, int depth, int parent, vector<int> &V)
{
	if(at <= n)
		V.push_back(depth);

	int i, sz = E[at].size();
	for(i = 0; i < sz; i++)
	{
		if(E[at][i].first == parent) continue;

		populate_depth(E[at][i].first, depth + E[at][i].second, at, V);
	}
}

typedef complex<double> base;
double PI = acos(-1.);
 
void fft (vector<base> & a, bool invert) {
	int n = (int) a.size();
 
	for (int i=1, j=0; i<n; ++i) {
		int bit = n >> 1;
		for (; j>=bit; bit>>=1)
			j -= bit;
		j += bit;
		if (i < j)
			swap (a[i], a[j]);
	}
 
	for (int len=2; len<=n; len<<=1) {
		double ang = 2*PI/len * (invert ? -1 : 1);
		base wlen (cos(ang), sin(ang));
		for (int i=0; i<n; i+=len) {
			base w (1);
			for (int j=0; j<len/2; ++j) {
				base u = a[i+j],  v = a[i+j+len/2] * w;
				a[i+j] = u + v;
				a[i+j+len/2] = u - v;
				w *= wlen;
			}
		}
	}
	if (invert)
		for (int i=0; i<n; ++i)
			a[i] /= n;
}

void multiply (const vector<int> & a, const vector<int> & b, vector<int> & res) {
	vector<base> fa, fb;
	int i;

	int sz = a.size();
	for(i = 0; i < sz; i++)
		fa.push_back( a[i] );
	
	sz = b.size();
	for(i = 0; i < sz; i++)
		fb.push_back( b[i] );

	size_t n = 1;
	while (n < MAX (a.size(), b.size()))  n <<= 1;
	n <<= 1;
	fa.resize (n),  fb.resize (n);
 
	fft (fa, false),  fft (fb, false);
	for (i=0; i<n; ++i)
		fa[i] *= fb[i];
	fft (fa, true);
 
	res.resize (n);
	for (i=0; i<n; ++i)
		res[i] = int (fa[i].real() + 0.5);
}

void f(vector<int> &X, vector<int> &Y)
{
	int i, maxx = 0;
	for(i = X.size() - 1; i >= 0; i--) maxx = MAX(X[i], maxx);
	for(i = Y.size() - 1; i >= 0; i--) maxx = MAX(Y[i], maxx);

	vector<int> A(maxx + 1, 0), B(maxx + 1, 0), C;
	for(i = X.size() - 1; i >= 0; i--) A[X[i]]++;
	for(i = Y.size() - 1; i >= 0; i--) B[Y[i]]++;


//	printf(">>>>\n");
//	for(i = A.size() - 1; i >= 0; i--) printf("%d, ", A[i]); printf("\n");
//	for(i = B.size() - 1; i >= 0; i--) printf("%d, ", B[i]); printf("\n");
	multiply(A, B, C);

//	for(i = C.size() - 1; i >= 0; i--) printf("%d, ", C[i]); printf("\n");

	maxx = C.size();
	int sz = Prime.size();
	for(i = 0; i < sz && Prime[i] < maxx; i++) 
		ans += C[Prime[i]];
}

queue<PII> Q;

void isTree()
{
	queue<int> Q;
	int vis[50004];
	int u, i;

	MEM(vis, 0);
	vis[1] = 1;
	Q.push(1);
	while(!Q.empty())
	{
		u = Q.front();
		Q.pop();

		for(i = V[u].size() - 1; i >= 0; i--)
			if(vis[V[u][i]]==0)
			{
				vis[V[u][i]] = 1;
				Q.push( V[u][i] );
			}
	}

	for(i = 1; i <= n; i++)
		assert(vis[i]);
}

int main()
{
	int i, a, b, j;
	PII u;
	vector<int> X, Y;

	sieve(50002);

	scanf("%d", &n);
	IN(n, 2, 50000);
	for(i = 1; i < n; i++)
	{
		scanf("%d %d", &a, &b);
		
		IN(a, 1, n);
		IN(b, 1, n);

		V[a].push_back(b);
		V[b].push_back(a);
	}

	isTree();

	dummy = n;

	//Make the tree binary. If there is a node with many children, keep one child left and make other children
	//the children of the right node. make the cost to right child 0. other 1.
	//In this tree, the cost of the path from an original node to another original node is same.
	Binarize(1, 0);

	
/*
		printf("%d\n", ans);
		for(i = 1; i <= dummy; i++)
		{
			printf("%d:", i);
			for(j = 0; j < E[i].size(); j++)
				printf(" (%d, %d)", E[i][j].first, E[i][j].second);
			printf("\n");
		}
*/

	//Now find center of the tree. center of the tree is a node, having <=n/3 nodes in each subtree if 
	//this node is deleted.
	//Now delete the link to its parent. For each subtree solve recursively.
	//For inter subtree path, list up all the distances to the original nodes in each subtree. Find the solution using FFT
	int x, y;

	int dd = 0;
	Q.push( PII(1, dummy) );
	while(!Q.empty())
	{
		u = Q.front();
		Q.pop();

		_sz = u.second;

		if(_sz == 1) continue;

		best_pos = -1;
		best_value = u.second;

		DFS(u.first, 0);

		x = best_pos;
		y = parent[x];

		E[x].erase( find(E[x].begin(), E[x].end(), PII(y, best_d)) );
		E[y].erase( find(E[y].begin(), E[y].end(), PII(x, best_d)) );

		Q.push( PII(x, subtree[x]) );
		Q.push( PII(y, _sz - subtree[x]) );

		X.clear();
		Y.clear();

		populate_depth(x, 0, 0, X);
		populate_depth(y, 0, 0, Y);

		if(best_d) for(i = X.size() - 1; i >= 0; i--) X[i]++;
		f(X, Y);
	}

//	printf("%d\n", ans);
	double f = (1.*ans)/( (n*(n-1.))/2 );
	printf("%.12lf\n", f);

	return 0;
}
