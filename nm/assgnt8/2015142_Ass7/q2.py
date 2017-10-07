import math
import matplotlib.pyplot as plt

h = [0.19, 0.20, 0.21]
a = 0
b = 2

for j in range(0,3):
	y0 = 1
	yp = []
	ye = []
	yie = []
	t = []
	t.append(a)
	yp.append(y0)
	ye.append(y0)
	yie.append(y0)


	N = int((b-a)/h[j])

	for i in range(1,N+1):
		t.append(t[i-1] + h[j])

		ye.append(ye[i-1] + h[j]*(-10 * ye[i-1]))

		yp.append(yie[i-1] + h[j]*(-10 * (yie[i-1])))
		yie.append(yie[i-1] + (h[j]/2)*(-10 * (yie[i-1]) + (-10 * yp[i])))

	plt.plot(t, ye, label = 'Eulers, h = ' + str(h[j]))
	plt.plot(t, yie, label = 'Improved Eulers, h = ' + str(h[j]))

h = [0.20, 0.30]
for j in range(0,2):
	y0 = 1
	ybe = []
	t = []

	t.append(a)
	ybe.append(y0)

	N = int((b-a)/h[j])
	for i in range(1,N+1):
		t.append(t[i-1] + h[j])
		ybe.append((ybe[i-1])/(1 + 10*h[j]))

	plt.plot(t, ybe, label = 'Backward Eulers, h = ' + str(h[j]))

h = 0.0001
N = int((b-a)/h)
t =[]
yreal = []
yreal.append(y0)
t.append(a)
for i in range(1,N+1):
	t.append(t[i-1] + h)
	yreal.append(math.exp((-10) * t[i]))

plt.plot(t, yreal, label = 'Yreal')

legend = plt.legend(bbox_to_anchor=(1.005, 1), loc=5, borderaxespad=0.)
frame = legend.get_frame()
frame.set_facecolor('0.90')
plt.savefig("q2.png")
plt.grid(True)
plt.show()