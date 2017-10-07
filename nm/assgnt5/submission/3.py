import numpy

n = int(input())
mat = []

for i in range(n):
	temp = []
	row = input().split()
	for j in row :
		temp.append(float(j))
	mat.append(temp)

b = list(map(float, input().split()))

b = numpy.matrix(b)
b = b.T

# print(b)

I = numpy.identity(n, float)

def get_U(mat, N):
	U = []
	for i in range(N):
		temp = [0 for j in range(N)]
		for j in range(N):
			if(j > i):
				temp[j] = mat[i][j]
		U.append(temp)
	return U

U = numpy.matrix(get_U(mat, n))

def get_L(mat, N):
	L = []
	for i in range(N):
		temp = [0 for j in range(N)]
		for j in range(N):
			if(j < i):
				temp[j] = mat[i][j]
		L.append(temp)
	return L

L = numpy.matrix(get_L(mat, n))


def calc_C():
	global I
	global U
	global L
	sum1 = I + L
	inverse1 = sum1.I
	C = inverse1 * U
	C = -1 * C
	return C

def get_const():
	global b
	sum1 = I + L
	inverse1 = sum1.I
	con = inverse1 * b
	return con

add1 = get_const()

# print(type(add1))

def check_error(new, now, n):
	for i in range(n):
		if(round(new[i, 0], 4) - round(now[i, 0], 4) > 0):
		# print (i)
			return 1
	return 0

def gauss_seidel(initial, n):
	global add1
	C = calc_C()
	now = initial
	new = (C * now) + add1
	counter = 1
	while(check_error(new, now, n)):
		now = new
		new = (C * now) + add1
		counter += 1

	return new, counter

initial = [[0.0] for i in range(n)]

initial = numpy.matrix(initial)

# print(initial)

ans, c = gauss_seidel(initial, n)

def display(ans, c, n):
	print("x = [", end = '')
	for i in range(n):
		print(round(ans[i, 0], 4), end = ' ')
	print("]")
	print("Counter = {}".format(c))

display(ans, c, n)

# print(C)
# print(L)
# print(U)
# print(I)