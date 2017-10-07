import math
import numpy as np
import matplotlib.pyplot as plt
import copy

def take_input():
	print("Enter the value of alpha : ", end = '')
	al = float(input())
	print("Enter the value of delta X: ", end = '')
	delX = float(input())
	print("Enter the length of the road: ", end = '')
	L = float(input())
	return al, delX, L

def f(x):
	PI = math.pi
	return math.sin(PI * x)
	
def make_initial_u(delX, L):
	u = []
	for i in np.arange(0, L, delX):
		u.append(f(i))
	u.append(0.0)
	print(u)
	return u

def make_matrix(r, delX, L):
	a = [-1*r for i in np.arange(0, L + delX, delX)]
	b = [(1 + 2*r) for i in np.arange(0, L + delX, delX)]
	c = [-1*r for i in np.arange(0, L + delX, delX)]
	a[0] = a[-1] = 0.0
	b[0] = b[-1] = 1.0
	c[0] = c[-1] = 0.0
	return a, b, c

def TDS_solver(a, b, c, d):
	n = len(b)
	delta = 1e-6
	for i in range(1, n):
		mul = a[i]/b[i-1]
		a[i] = a[i] - (mul)*b[i-1]
		b[i] = b[i] - (mul)*c[i-1]
		if(b[i] == 0):
			return False
		d[i] = d[i] - (mul)*d[i-1]
	d[n-1] = d[n-1]/b[n-1]
	for j in range(n-2, -1, -1):
		d[j] = (d[j]-(c[j]*d[j+1]))/b[j]
	return d

def exact_f(x, t):
	PI = math.pi
	ans = round(math.sin(PI*x), 8)
	ans *= math.exp(-1 * PI * PI * t)
	return ans;

def plot_exact(L, t):
	dx = 0.001
	X = np.arange(0, L + dx, dx)
	Y = []
	for i in X:
		Y.append(exact_f(i, t))
	return X, Y

def error(exact, implicit):
	ans = math.fabs(exact - implicit)
	return round(ans, 8)

def pretty_print_header():
	space = "--"
	print("{!s:^10} {!s:^4} {!s:^10} {!s:^4} {!s:^10} {!s:^4} {!s:^10} {!s:^4} {!s:^10}".format("time", space , "x", space , "exact", space, "implicit", space, "error"))

def pretty_print(t, x, exact, implicit):
	e = error(exact, implicit)
	space = "--"
	print("{!s:10.9} {!s:^4} {!s:10.9} {!s:^4} {!s:10.9} {!s:^4} {!s:10.9} {!s:^4} {!s:10.9}".format(t, space , x, space , exact, space, implicit, space, e))

def iteration_scheme():
	al, delX, L = take_input()
	r = 0.5
	delT = round((r * delX * delX)/al, 6)
	print("delT = ", delT)
	n = L/delX
	u_n = make_initial_u(delX, L)
	a, b, c = make_matrix(r, delX, L)
	x = np.arange(0, delX + L, delX)
	pretty_print_header()
	fig = plt.figure()
	for i in np.arange(0, 0.2 + delT, delT):
		a, b, c = make_matrix(r, delX, L)
		if(i in [0, 0.04, 0.08, 0.12, 0.16, 0.2]):
			print("----------------------------------------------------------------------")
			for index, j in enumerate(u_n):
				pretty_print(i, x[index], exact_f(x[index], i), j)
			plt.plot(x, u_n, label = '%f' % i)
		next1 = copy.copy(TDS_solver(a, b, c, u_n))
		u_n = copy.copy(next1)
	for i in np.arange(0, 0.2 + delT, delT):
		if(i in [0, 0.04, 0.08, 0.12, 0.16, 0.2]):
			x, y = plot_exact(L, i)
			plt.plot(x, y, '--', label = 'exact t = %f' % i)
	fig.suptitle('Implicit method', fontsize=20)
	plt.legend(bbox_to_anchor=(0.8, 1.05), loc=2, borderaxespad=0.)
	fig.savefig('implicit.png')
	plt.show()

iteration_scheme()