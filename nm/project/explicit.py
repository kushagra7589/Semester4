import math
import matplotlib.pyplot as mpl
import numpy

def next(u0,u1,u2,r):
	return u1 + r*(u2 - 2*u1 + u0)

def plot(x,u,r,t,n,delT):
	unext = list(u)
	for j in range(int(t/delT)):
		u = list(unext)
		for i in range(1,n):
			unext[i]=next(u[i-1],u[i],u[i+1],r)
	mpl.plot(x,unext,label="value at t={0:.2f}".format(t))
	return unext

def actual(L, t, delX):
	x = numpy.arange(0.000,L+delX,delX)
	y = list()
	for i in x:
		y.append(float(format(numpy.sin(numpy.pi*i)*numpy.exp(-1*t*numpy.pi**2),'.5f')))
	mpl.plot(x,y,linestyle=':',label="exact value at t={0:.2f}".format(t))
	return y

def FTCSExplicit(L,delX,alfa):
	delT=delX**2/2*alfa #Max DeltaT
	n = int(L/delX)
	u = list()
	x = list()
	for i in range(0,n+1):
		x.append(delX*i)
		u.append(float(format(numpy.sin(numpy.pi*x[i]),'.5f')))
	mpl.plot(x,u,label='value at t=0')
	calculated = actual(L,0.00,delX)
	result = list(u)

	r = alfa*delT/delX**2
	
	time = [0.04,0.08,0.12,0.16,0.20]
	print("\ttime\t--\tx\t\t--\texact\t\t--\t\texplicit\t--\t\terror")
	print("--------------------------------------------------------------------------------------------------------------------------------------------")
	for i in range(len(result)):
		print("\t0.00\t--\t"+"{0:.2f}".format(x[i])+"\t\t--\t"+"{0:.5f}".format(result[i])+"\t\t--\t\t"+"{0:.5f}".format(calculated[i])+"\t\t--\t\t"+"{0:.5f}".format(calculated[i]-result[i]))
	
	for t in time:
		result = actual(L, t, delX)
		calculated = plot(x,u,r,t,n,delT)
		print("--------------------------------------------------------------------------------------------------------------------------------------------")
		for i in range(len(result)):
			print("\t"+"{0:.2f}".format(t)+"\t--\t"+"{0:.2f}".format(x[i])+"\t\t--\t"+"{0:.5f}".format(result[i])+"\t\t--\t\t"+"{0:.5f}".format(calculated[i])+"\t\t--\t\t"+"{0:.5f}".format(calculated[i]-result[i]))
	# plot(x,u,r,0.08,n,delT)
	# plot(x,u,r,0.12,n,delT)
	# plot(x,u,r,0.16,n,delT)
	# plot(x,u,r,0.20,n,delT)

if __name__ == '__main__':
	L=float(input("Length:	"))
	delX=float(input("Delta X:	"))
	alfa=float(input("Alpha:	"))
	FTCSExplicit(L,delX,alfa)

mpl.legend(loc='upper center', bbox_to_anchor=(0.5, 1.1), ncol=4, fancybox=True, shadow=True)
mpl.show()