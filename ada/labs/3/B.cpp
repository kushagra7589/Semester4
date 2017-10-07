#include <bits/stdc++.h>

using namespace std;

#define ld int
#define mp make_pair
#define pb push_back
#define F first
#define S second
#define pll pair<ld, ld>

// vector<pll > strip;

const int MAX_SIZE = 1e5+10;
const ld INF = 1e9;
pll C[MAX_SIZE];
pll D[MAX_SIZE];

ld dist(pair<ld, ld> &a, pair<ld, ld> &b)
{
	return (a.F - b.F)*(a.F - b.F) + (a.S - b.S)*(a.S - b.S);
}

ld ClosestPair(pll Px[] , pll Py[], int n)
{
	if(n <= 3)
	{
		ld res = INF;
		for(int i=0; i<n; i++)
		{
			for(int j=i+1; j<n; j++)
			{
				res = min(res, dist(Px[i], Px[j]));
			}
		}
		return res;
	}

	int mid = n/2;

	pll Pyl[mid + 10];
	pll Pyr[n - mid + 10];

	int li=0, ri=0;
	for(int i=0; i<n; i++)
	{
		if(Py[i].S <= Px[mid].F)
		{
			Pyl[li++] = Py[i];
		}
		else
		{
			Pyr[ri++] = Py[i];
		}
	}

	ld left = ClosestPair(Px, Pyl, li);
	ld right = ClosestPair(Px + mid, Pyr, ri);
	ld d = min(left, right);
	vector<pll > strip;
	for(int i=0; i<n; i++)
	{
		if(abs(Px[mid].F -  Py[i].S) < d)
		{
			strip.pb(Py[i]);
		}
	}

	ld min_d = d;
	for(int i=0; i<strip.size(); i++)
	{
		for(int j=i+1; j<strip.size() && ((strip[j].F - strip[i].F) < min_d); j++)
		{
			min_d = min(min_d, dist(strip[i], strip[j]));
		}
	}

	return min_d;
}

int main()
{
	ios_base::sync_with_stdio(false);
	int N;
	cin >> N;
	// C.resize(N);
	ld x, y;
	for(int i=0; i<N; i++)
	{
		cin >> x >> y;
		C[i] = mp(abs(x), abs(y));
		D[i] = mp(abs(y), abs(x));
	}
	sort(C, C + N);
	sort(D, D + N);
	long long ans = ClosestPair(C, D, N);
	cout << ans << endl;
	return 0;
}