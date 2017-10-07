import random
import pylab as plt

def L_j(j, xi, n, X):
	ans = 1.0
	for i in range(n):
		if i != j:
			ans *= (xi - X[i])/(X[j] - X[i])
	return ans

def interpolate(X, F, X_I):
	n = len(X)
	L = []
	color = ['r', 'g', 'b', 'c']
	for i in range(n):
		tempL = []
		for xi in X_I:
			tempL.append(L_j(i, xi, n, X))
		# plt.plot(X_I, tempL, color[i], label='L_'+str(i))
		L.append(tempL)
	return L

X_I = [random.uniform(0.0, 3.0) for i in range(100)]
X_I = sorted(X_I)
X_I = [0.5]
x = [0, 1, 2, 3]
f = [1, 0.765198, 0.223891, -0.260052]

ansL = interpolate(x, f, X_I)

n = len(ansL[0])
LY = [0.0 for i in range(n)]

for i in range(n):
	for index, j in enumerate(ansL):
		LY[i] += f[index] * j[i]

print("P3(x) = ", LY)

# plt.plot(X_I, LY, 'k', label='p_3(x)')
# plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.1),
          # ncol=3, fancybox=True, shadow=True)
# plt.savefig('main.png')
# plt.show()