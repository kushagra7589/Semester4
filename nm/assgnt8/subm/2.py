import math
import matplotlib.pyplot as plt

def euler(f,y0,a,b,h):
	t,y = a,y0
	X = []
	Y = []
	X.append(t)
	Y.append(y0)
	while t <= b:
		print "x: %6.6f y: %6.6f real:%6.6f error:%6.6f" % (t,y, realF(t), abs(realF(t) - y))
		# print "error : %f" % abs(realF(t) - y)
		t += h
		y += h * f(t,y)
		X.append(t)
		Y.append(y)
	return X, Y

def realF(x):
	return math.exp(-10*x)

def f(y, x):
	return -10 * y

X, Y = euler(f, 1, 0, 2, 0.19)
plt.plot(X, Y)
X, Y = euler(f, 1, 0, 2, 0.20)
plt.plot(X, Y)
X, Y = euler(f, 1, 0, 2, 0.21)
plt.plot(X, Y)
plt.savefig("20.png")
# plt.savefig("21.png")
# plt.savefig("19.png")
# plt.show()
# plt.show()
plt.show()