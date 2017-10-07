import math
h = float(input("Enter step size h: "))
# h = 0.1
a = 0
b = 1
y0 = 0
yp = []
yc = []
x = []
yreal = []
x.append(a)
yp.append(y0)
yreal.append(y0)
yc.append(y0)
N = int((b-a)/h)

for i in range(1,N+1):
	x.append(x[i-1] + h)
	yp.append(yc[i-1] + h*((x[i-1] + yc[i-1])**2))
	yc.append(yc[i-1] + (h/2)*((x[i-1] + yc[i-1])**2+ (x[i] + yp[i])**2))
	yreal.append(math.tan(x[i]) - x[i])

for i in range(0,N+1):
	print ("X coordinate: ",round(x[i], 3), "Y coordinate: ", round(yc[i], 7), "Y exact = ", round(yreal[i],7), "Error = ", round(yreal[i] - yc[i], 7))
