#input : n * (n+1) matrix

n = int(input())
matrix = []

for i in range(n) :
	temp = [] 
	row = input().split();
	for j in row :
		temp.append(float(j))
	matrix.append(temp)

def display(matrix, n):
	for i in range(n) :
		for j in range(n+1) :
			print ("{:.5f}".format(matrix[i][j]), end = ' ')
		print()

def getMaxAbove(startRow, column, matrix, n) :
	maxEle = -1e9
	maxRow = startRow
	for j in range(startRow, n):
		if(abs(matrix[j][column]) > maxEle):
			maxEle = abs(matrix[j][column])
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
		print("There are infinitely many solutions.")
		return False
	matrix = getMaxAbove(pivot, pivot, matrix, n)
	for j in range(pivot+1, n):
		scale=matrix[j][pivot]/matrix[pivot][pivot]
		if(matrix[j][pivot] != 0):
			for k in range(pivot, n+1):
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
		print("There are no solutions.")
		return False

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

matrix = RREF(matrix, n)
if(matrix):
	display(matrix, n)