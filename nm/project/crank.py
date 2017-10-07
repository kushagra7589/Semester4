import math
import numpy
import matplotlib.pyplot as plt
def Thomas(an,bn,cn,n,b):
	for i in range(1,n):
		m = an[i]/bn[i-1]
		an[i] = an[i]-m*bn[i-1]
		bn[i] = bn[i]-m*cn[i-1]
		# print(m*bn[i])
		b[i] = b[i]-m*b[i-1]
			
	xn=[0 for i in range(0,n)]
	xn[n-1]=b[n-1]/bn[n-1]
	i=n-2
	while(i>=0):
		xn[i] = (b[i]-cn[i]*xn[i+1])/bn[i]
		i-=1
	return xn
def f(x):
	return math.sin(math.pi*x)

def Crank_Nicholsan(delta_x,delta_t,r):
	x=numpy.arange(0,1.01,delta_x)
	t=numpy.arange(0,0.21,delta_t)
	print(len(t))
	n=len(x)
	Matrix = [[0 for x in range(len(x))] for y in range(len(t))] 
	for i in range(len(x)-1):
		Matrix[0][i]=f(x[i])
	Matrix[0][n-1]=0	
	for j in range(1,len(t)):
		an=[]
		bn=[]
		cn=[]
		b=[]
		an.append(0)
		bn.append(1)
		cn.append(0)
		b.append(0)
		for i in range(1,n-1):
			an.append(-r)
			bn.append(1+2*r)
			cn.append(-r)	
		an.append(0)
		bn.append(1)	
		cn.append(0)	
		for i in range(1,n-1):
			# print(Matrix[j-1][i+1])	
			b.append(Matrix[j-1][i])
		b.append(0)	
		ans=Thomas(an,bn,cn,n,b)
		# print(ans)
		for i in range(0,len(ans)):
			Matrix[j][i]=ans[i]

	print(Matrix)
	plt.figure(1)
	i=0
	while(i<11):
		aux=Matrix[i]
		plt.plot(x,aux,label=t[i])
		i+=2
	plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.05),
          ncol=3, fancybox=True, shadow=True)
	plt.show()
Crank_Nicholsan(0.2,0.02,0.5)	