#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <complex>
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
//#include <ctime>
#include <map>
using namespace std;

// FFT on complex filed. (You can also use FFT on finite field.)
complex <double> x[1<<20];
complex <double> y[1<<20];
complex <double> r[1<<20];

namespace Fourier
{
	// n must be 2^k
	complex <double> t[1<<20];
	void FFT(complex <double> *array, int n, double pi = M_PI)
	{
		for(int step = n>>1; step >= 1; step >>= 1)
		{
			complex <double> w(cos(2 * pi * step / n), sin(2 * pi * step / n));
			for(int start = 0; start < step; start ++)
			{
				complex <double> factor(1, 0);
				int z = 0;
				for(int i = start; i < n; i += step << 1)
				{	
					array[i+step] *= factor;
					array[i] += array[i+step];
					array[i+step] = array[i] - array[i+step] - array[i+step];
					factor *= w;
					t[z] = array[i];
					t[z+n/step/2] = array[i+step];
					z ++;
				}
				z = 0;
				for(int i = start; i < n; i += step)
					array[i] = t[z++];
			}
		}
		for(int i = 0; i < n; i++)
			array[i] *= complex <double> (1/sqrt(n), 0);
	}

	void IFFT(complex <double> *array, int n)
	{
		FFT(array, n, -M_PI);
	}

	void cyclic_convolution(complex <double> *A, complex <double> *B, complex <double> *result, int n)
	{
		FFT(A, n);
		FFT(B, n);
		for(int i = 0; i < n; i++)
			result[i] = A[i] * B[i];
		IFFT(result, n);
		for(int i = 0; i < n; i++)
			result[i] *= complex <double> (sqrt(n), 0);
	}
}

int n;
const int MAXN = 200000;
const int MAXDEPTH = 20;
vector <int> e[MAXN];
int cntNodes[MAXN];
int minValue, center;

vector <int> cache[MAXDEPTH][4];
int cache_ans[MAXDEPTH][4][MAXN];
int ans[MAXN];
int temp1[MAXN];
int temp2[MAXN];
int temp3[MAXN];
int temp[MAXN];
int saveN[MAXDEPTH];

// find the minimal n such that 2**n > x
int minimalPow2(int x)
{
	int ret = 0;
	int t = 1;
	while(t < x)
	{
		ret ++;
		t = t + t;
	}
	return ret;
}

// use dfs to find the center of a tree
int dfs(int cur, int from, int d, int n, bool special = false, int depth = -1)
{
	if(special)
		cache_ans[depth][0][d] ++; 
	cntNodes[cur] = 1;
	int maxSubTree = 0, cnt = 0;
	for(int i = 0; i < e[cur].size(); i++)
		if(e[cur][i] != from)
		{

			int t = dfs(e[cur][i], cur, d+1, n, special, depth);
			cntNodes[cur] += cntNodes[e[cur][i]];
			cnt += t;
			maxSubTree = max(maxSubTree, t);
		}
	maxSubTree = max(maxSubTree, n-1-cnt);
	if(n > 0 && maxSubTree < minValue)
	{
		center = cur;
		minValue = maxSubTree;
	}
	return cnt + 1;
}

int totNodes;

int findCenter(int root, int depth)
{
	int n = dfs(root, -1, 0, 0, true, depth);
	minValue = 100000000;
	totNodes = dfs(root, -1, 0, n, false, depth);
	dfs(center, -1, 0, n, false, depth);
	return center;
}

// A[i]: number of nodes have a distance i from subtree A
// B[i]: number of nodes have a distance i from subtree B
// Update all pathes between subtree A and subtree B
void calcPair(int *A, int *B, int N)
{
	for(int i = 0; i < (1<<N); i++)
	{
		x[i] = complex <double> (A[i], 0);
		y[i] = complex <double> (B[i], 0);
	}
	Fourier :: cyclic_convolution(x, y, r, 1<<N);

	for(int i = 1; i < (1<<N); i++)
		ans[i] += (long long)(r[i].real() + 0.5);
}

void solve(int root, int depth, int reqN)
{
	for(int i = 0; i < (1<<reqN); i++)
	{
		cache_ans[depth][0][i] = 0;
		cache_ans[depth][1][i] = 0;
		cache_ans[depth][2][i] = 0;
		cache_ans[depth][3][i] = 0;
	}
	int center = findCenter(root, depth);
	int totalNodes = cntNodes[center];

	cache_ans[depth][0][0] = 1;
	if(totalNodes <= 2)
	{
		if(totalNodes == 2)
		{
			cache_ans[depth][0][1] = 1;
			ans[1] ++;
		}
		return;
	}
	cache[depth][0] = e[center];
	cache[depth][1].clear(), cache[depth][2].clear(), cache[depth][3].clear();
	int cnt = 1;

	// The subtree will be grouped into 3 parts, each one has a size no more than N/2.
	for(int i = 0; i < e[center].size(); i++)
	{
		int before = cnt;
		int after = cnt + cntNodes[e[center][i]];
		cnt = after;
		if(after <= totalNodes / 2)
			cache[depth][1].push_back(e[center][i]);
		else if(before <= totalNodes / 2)
			cache[depth][2].push_back(e[center][i]);
		else
			cache[depth][3].push_back(e[center][i]);
	}
	int N = minimalPow2(totalNodes);
	N = max(N, 2);

	e[center] = cache[depth][1];
	solve(center, depth + 1, N);
	for(int i = 1; i < (1<<N); i++)
		cache_ans[depth][1][i] = cache_ans[depth+1][0][i];
	
	e[center] = cache[depth][2];
	solve(center, depth + 1, N);
	for(int i = 1; i < (1<<N); i++)
		cache_ans[depth][2][i] = cache_ans[depth+1][0][i];
	
	e[center] = cache[depth][3];
	solve(center, depth + 1, N);
	for(int i = 1; i < (1<<N); i++)
		cache_ans[depth][3][i] = cache_ans[depth+1][0][i];
	
	e[center] = cache[depth][0];

	for(int i = 0; i < (1<<reqN); i++)
	{
		temp1[i] = cache_ans[depth][1][i];
		temp2[i] = cache_ans[depth][2][i];
		temp3[i] = cache_ans[depth][3][i];
	}
	calcPair(temp1, temp2, reqN);
	calcPair(temp1, temp3, reqN);
	calcPair(temp2, temp3, reqN);
}

bool isPrime[1000001];


int MAIN()
{
	memset(isPrime, true, sizeof(isPrime));
	isPrime[0] = isPrime[1] = false;
	for(int i = 2; i <= 1000000; i++)
		if(isPrime[i])
			for(int j = i+i; j <= 1000000; j+=i)
				isPrime[j] = false;

	memset(ans, 0, sizeof(ans));
	memset(cache_ans, 0, sizeof(cache_ans));
	cin >> n;
	for(int i = 1; i < n; i++)
	{
		int a, b;
		cin >> a >> b;
		e[a].push_back(b);
		e[b].push_back(a);
	}
	solve(1, 0, minimalPow2(n));
	
	double total = 0, sum = 0;
	for(int i = 1; i <= n; i++)
	{
		total += ans[i];
		if(isPrime[i])
			sum += ans[i];
	}
	cout << sum / total << endl;
	return 0;
}


int main()
{
	//srand((unsigned)time(NULL));
	#ifdef LOCAL_TEST
		freopen("in.txt", "r", stdin);
		freopen("out.txt", "w", stdout);
	#endif
	ios :: sync_with_stdio(false);
	cout << fixed << setprecision(16);
	return MAIN();
}
