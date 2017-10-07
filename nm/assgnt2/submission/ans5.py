import random
import pylab as plt

#create random data points to generate groph
X_I = [random.uniform(-5.0, 5.0) for i in range(500)]
X_I = sorted(X_I)

#create actual function
F_actual = [1.0/(1.0 + i**2) for i in X_I]

def function(xi):
	return 1.0/(1.0 + xi**2)

#create data points to interpolate function
x = [float(i) for i in range(-5, 6)]
f = [1.0/(1.0 + i**2) for i in x]

n = 10
matrix = [[] for i in range(n+1)]
for i in matrix:
	for j in range(n+2):
		i.append(0)

# for i in matrix:
# 	for j in i :
# 		print(j, end=' ')
# 	print()

for i in range(1, n) :
	# print(i, end=' ')
	matrix[i][i-1] = 1.0/(x[i] - x[i-1])
	matrix[i][i] = 2.0 * (1/(x[i] - x[i-1]) + 1/(x[i+1] - x[i]))    
	matrix[i][i+1] = 1.0/(x[i+1] - x[i])      
	matrix[i][n+1] = 3.0*((f[i]-f[i-1])/((x[i] - x[i-1])*(x[i] - x[i-1])) +  (f[i+1]-f[i])/((x[i+1] - x[i])*(x[i+1] - x[i])))

matrix[0][0] = 2.0/(x[1] - x[0])
matrix[0][1] = 1.0/(x[1] - x[0])
matrix[0][n+1] = 3.0 * (f[1] - f[0])/((x[1]-x[0])*(x[1]-x[0]))
matrix[n][n-1] = 1.0/(x[n] - x[n-1])
matrix[n][n] = 2.0/(x[n] - x[n-1])
matrix[n][n+1] = 3.0 * (f[n] - f[n-1])/((x[n]-x[n-1])*(x[n]-x[n-1]))

n = n+1
def display(matrix, n):
	for i in range(n) :
		for j in range(n+1) :
			print ("{:.10f}".format(matrix[i][j]), end = ' ')
		print()

def getMaxAbove(startRow, column, matrix, n) :
	maxEle = -1e9
	maxRow = startRow
	for j in range(startRow, n):
		if(matrix[j][column] > maxEle):
			maxEle = matrix[j][column]
			maxRow = j
	if(startRow != maxRow):
		temp = matrix[startRow]
		matrix[startRow] = matrix[maxRow]
		matrix[maxRow] = temp
	return matrix

def forwardStep(matrix, n, pivot):
	flag = 0
	for k in range(pivot, n):
		if(matrix[k][pivot] != 0):
			flag = 1
	if(not flag):
		return False
	matrix = getMaxAbove(pivot, pivot, matrix, n)
	for j in range(pivot+1, n):
		scale=matrix[j][pivot]/matrix[pivot][pivot]
		if(matrix[j][pivot] != 0):
			for k in range(pivot, n+1):
				matrix[j][k] = matrix[j][k] - (scale)*(matrix[pivot][k])
	return matrix

def backSubstitution(matrix, n, pivot):
	for i in range(pivot-1, -1, -1) :
		scale = matrix[i][pivot]/matrix[pivot][pivot]
		for j in range(pivot, n+1):
			matrix[i][j] = matrix[i][j] - (scale)*(matrix[pivot][j])
	return matrix

def RREF(matrix, n):
	#forward step
	for i in range(n):
		matrix = forwardStep(matrix, n, i)
		if(matrix == False):
			print ("Error")
			return False
	#scaling
	for i in range(n):
		scale = matrix[i][i]
		for j in range(n+1):
			matrix[i][j] /= scale

	#back subsitution
	for i in range(n-1, -1, -1):
		matrix = backSubstitution(matrix, n, i)

	return matrix

# display(matrix, n)
# print()
matrix = RREF(matrix, n)
# if(matrix):
# 	display(matrix, n)
K = []
for i in range(n):
	K.append(matrix[i][n])

def cubic_spline(xi):
	i = 1
	while(x[i]<xi):
		i+=1
	t = (xi - x[i-1]) / (x[i] - x[i-1])	
	a =  K[i-1]*(x[i]-x[i-1]) - (f[i]-f[i-1])
	b = -K[i]*(x[i]-x[i-1]) + (f[i]-f[i-1])		
	q = (1-t)*f[i-1] + t*f[i] + t*(1-t)*(a*(1-t)+b*t)
	return q

max_devg = 0
for i in range(n-1):
	start = x[i]
	end = x[i+1]
	tempX = [random.uniform(start, end) for j in range(50)]
	cubicY = [cubic_spline(j) for j in tempX]
	for j in tempX:
		max_devg = max(max_devg, abs(function(j)-cubic_spline(j)))
	if(i == 0):
		plt.plot(tempX, cubicY, 'b',label="g(x)")
	else:
		plt.plot(tempX, cubicY, 'b')

print("Max deviation in g_x: ", max_devg)

def L_j(j, xi, n, X):
	ans = 1.0
	for i in range(n):
		if i != j:
			ans *= (xi - X[i])/(X[j] - X[i])
	return ans

def interpolate(X, F, X_I):
	n = len(X)
	L = []
	# color = ['r', 'g', 'b', 'c']
	for i in range(n):
		tempL = []
		for xi in X_I:
			tempL.append(L_j(i, xi, n, X))
		# plt.plot(X_I, tempL, label='L_'+str(i))
		L.append(tempL)
	return L

ansL = interpolate(x, f, X_I)

n = len(ansL[0])
LY = [0.0 for i in range(n)]

for i in range(n):
	for index, j in enumerate(ansL):
		LY[i] += f[index] * j[i]

max_devp = 0
for index, i in  enumerate(LY):
	max_devp = max(max_devp, abs(F_actual[index] - i))

print("Max deviation in p(x): ", max_devp)

plt.plot(X_I, LY, 'k', label='p_10(x)')
plt.plot(X_I, F_actual, 'r', label='f(x)')
plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.1),
		  ncol=3, fancybox=True, shadow=True)
plt.savefig('ans5interpolation.png')
plt.show()

