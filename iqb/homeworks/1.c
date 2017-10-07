#include <stdio.h>
#include <string.h>

#define MAX_SIZE 100001
#define Kb 1
#define epsilon 1
#define T 1
#define ll long long


int getMapping(char c)
{
	if(c == 'A' || c == 'T') return 0;
	return 1;
}

ll bondingEnergy(int i)
{
	ll ans;
	if(i) ans = 3;
	else ans = 2;
	return ans * Kb * epsilon * T;
}

ll totalBondingEnergy(int *A, int n)
{
	int i=0;
	ll total_energy = 0;
	for(i=0; i<n; i++)
	{
		total_energy += bondingEnergy(A[i]);
	}
	return total_energy;
}


int main()
{
	char *dna;
	scanf("%s", dna);
	int n = strlen(dna);
	int mapping[MAX_SIZE];
	int i=0;
	for(i = 0; i<n; i++)
	{
		mapping[i] = getMapping(dna[i]);
	}
	ll total_energy = totalBondingEnergy(mapping, n);
	printf("The bonding energy is %lld\n", total_energy);
	return 0;
}