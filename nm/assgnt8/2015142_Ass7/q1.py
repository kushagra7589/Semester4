import math
h = float(input("Enter step size h: "))
# h = 0.1
a = 0
b = 1
y0 = 0
y = []
x = []
yreal = []
x.append(a)
y.append(y0)
yreal.append(y0)
N = int((b-a)/h)

for i in range(1,N+1):
	x.append(x[i-1] + h)
	y.append(y[i-1] + h*((x[i-1] + y[i-1])**2))
	yreal.append(math.tan(x[i]) - x[i])

for i in range(0,N+1):
	print ("X coordinate: ",round(x[i], 3), "Y coordinate: ", round(y[i], 5), "Y exact = ", round(yreal[i],5), "Error = ", round(yreal[i] - y[i], 5))
