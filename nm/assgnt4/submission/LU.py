#input : n * n matrix
#		 vector b

n = int(input())
matrix = []

for i in range(n) :
	temp = [] 
	row = input().split();
	for j in row :
		temp.append(float(j))
	matrix.append(temp)

b = list(map(float, input().split()))

L = []

for i in range(n):
	temp = [0 for j in range(n)]
	L.append(temp)

# print (L)

def display(matrix, n):
	for i in range(n) :
		for j in range(n) :
			print ("{:.5f}".format(matrix[i][j]), end = ' ')
		print()

def displayL(n):
	global L
	for i in range(n) :
		for j in range(n) :
			print ("{:.5f}".format(L[i][j]), end = ' ')
		print()

def getMaxAbove(startRow, column, matrix, n) :
	global L
	global b
	maxEle = -1e9
	maxRow = startRow
	for j in range(startRow, n):
		if(abs(matrix[j][column]) > maxEle):
			maxEle = abs(matrix[j][column])
			maxRow = j
	if(startRow != maxRow):
		temp = matrix[startRow]
		tempL = L[startRow]
		tempB = b[startRow]
		matrix[startRow] = matrix[maxRow]
		L[startRow] = L[maxRow]
		b[startRow] = b[maxRow]
		matrix[maxRow] = temp
		L[maxRow] = tempL
		b[maxRow] = tempB
	return matrix

def forwardStep(matrix, n, pivot):
	flag = 0
	global L
	for k in range(pivot, n):
		if(matrix[k][pivot] != 0):
			flag = 1
	if(not flag):
		print("There are infinite solutions.")
		return False
	matrix = getMaxAbove(pivot, pivot, matrix, n)
	for j in range(pivot+1, n):
		scale=matrix[j][pivot]/matrix[pivot][pivot]
		L[j][pivot] = scale
		# print("Pivot = ", pivot, " J = ", j, " scale = ", scale)
		if(matrix[j][pivot] != 0):
			for k in range(pivot, n):
				matrix[j][k] = matrix[j][k] - (scale)*(matrix[pivot][k])
	#error due  to loss of precision while using floating point numbers
	delta = 1e-5
	flag = 0
	for index, i in enumerate(matrix[pivot]):
		if index != n:
			if not(-delta <= i <= delta) :
				flag = 1
		else:
			if(not flag):
				if(-delta <= i <= delta):
					flag = 1
	if(flag):
		return matrix
	else:
		print("There is no solution")
		return False

# def backSubstitution(matrix, n, pivot):
# 	for i in range(pivot-1, -1, -1) :
# 		scale = matrix[i][pivot]/matrix[pivot][pivot]
# 		for j in range(pivot, n+1):
# 			matrix[i][j] = matrix[i][j] - (scale)*(matrix[pivot][j])
# 	return matrix

def forwardSubstitution(b, n):
	global L
	c = [0 for i in range(n)]
	for i in range(n):
		c[i] = b[i]
		for j in range(i-1, -1, -1):
			c[i] -=  L[i][j]*c[j]
	return c

def backSubstitution(matrix, c, n):
	x = [0 for i  in range(n)]
	for i in range(n-1, -1, -1):
		x[i] = c[i]
		for j in range(i+1, n):
			x[i] -= matrix[i][j]*x[j]
		x[i] /= matrix[i][i]
	return x

def calcU(matrix, n):
	#forward step
	for i in range(n):
		matrix = forwardStep(matrix, n, i)
		if(matrix == False):
			# print ("Error")
			return False

	#back subsitution
	# for i in range(n-1, -1, -1):
	# 	matrix = backSubstitution(matrix, n, i)

	return matrix


U = calcU(matrix, n)
print()
if(U):
	print("U = ")
	display(U, n)
	for i in range(n):
		L[i][i] = 1
	print()
	print("L = ")
	displayL(n)
	c = forwardSubstitution(b, n)
	x = backSubstitution(U, c, n)
	print()
	for index, i in enumerate(x):
		print("x_{} = {:.5f}".format(index, i))