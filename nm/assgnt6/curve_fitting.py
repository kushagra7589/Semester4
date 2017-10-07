import numpy as np

n = int(input())		#number of data points

x = list(map(float, input().split()))
y = list(map(float, input().split()))

d = int(input())		#degree of curve fitting

X = [0 for i in range(2*d + 1)]			#to store sigma(1), sigma(x), sigma(x^2)..
Y = [0 for i in range(d + 1)]			#to store sigma(y), sigma(xy), sigma(x^2y)..

for i in range(2*d + 1):
	for j in range(n):
		X[i] = X[i] + (x[j] ** i)

for i in range(d + 1):
	for j in range(n):
		Y[i] = Y[i] + (x[j]**i)*y[j]

B = [[] for i in range(d+1)]

for i in range(d+1):
	B[i] = [X[i + j] for j in range(d+1)]

b = [Y[i] for i in range(d + 1)]

B = np.array(B)
b = np.array(b)

x = np.linalg.solve(B, b)

for i in x :
	print(i)