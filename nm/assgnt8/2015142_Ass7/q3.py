import math
import matplotlib.pyplot as plt

h = [0.1, 0.2]
a = 0
b = 2


print ("Forward Euler Method : ")
for j in range(0,2):
	x = [[]]
	xreal = []
	xreal.append(1)
	N = int((b-a)/h[j])
	for k in range(0,2):
		x.append([])
	for k in range(0,N+1):
		x[0].append(0)
		x[1].append(1)

	x[0][0] = 1
	x[1][0] = -10

	t = []
	t.append(a)

	for i in range(1,N+1):
		t.append(t[i-1] + h[j])
		x[0][i] = x[0][i-1] + h[j]*(x[1][i-1])
		x[1][i] = x[1][i-1] + h[j]*(100*x[0][i-1])
		xreal.append(math.exp(-10*t[i]))

	print ("h= ", h[j])
	for i in range(0,N+1):
		print("T= ", round(t[i], 3), "X= ", round(x[0][i], 10), "Xreal= ", round(xreal[i], 10), "Error= ", round(x[0][i] - xreal[i], 10))


print ("Backward Euler Method : ")
print ("h = 0.2 ")
h = 0.2
s = -1/3


x = [[]]
xreal = []
xreal.append(1)
N = int((b-a)/h)
for k in range(0,2):
	x.append([])
for k in range(0,N+1):
	x[0].append(0)
	x[1].append(1)

x[0][0] = 1
x[1][0] = -10


t = []
t.append(a)
for i in range(1,N+1):
	t.append(t[i-1] + h)
	x[0][i] = s * (x[0][i-1] + h*(x[1][i-1]))
	x[1][i] = s * (100*h*x[0][i-1] + x[1][i-1])
	xreal.append(math.exp(-10*t[i]))
for i in range(0,N+1):
	print("T= ", round(t[i], 3), "X= ", round(x[0][i], 10), "Xreal= ", round(xreal[i], 10), "Error= ", round(x[0][i] - xreal[i], 10))
