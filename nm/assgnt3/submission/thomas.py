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

a = list(map(float, input().split()))
b = list(map(float, input().split()))
c = list(map(float, input().split()))
d = list(map(float, input().split()))

ans = TDS_solver(a, b, c, d)

for index, i in enumerate(ans):	
	print("x" + str(index), " = ", i)