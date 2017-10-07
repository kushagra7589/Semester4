import math

def euler(f,y0,a,b,h):
	t,y = a,y0
	while t <= b:
		print "x: %6.6f y: %6.6f real:%6.6f error:%6.6f" % (t,y, realF(t), abs(realF(t) - y))
		# print "error : %f" % abs(realF(t) - y)
		t += h
		y += h * f(t,y)
 
def realF(x):
	return math.tan(x) - x

def f(y, x):
	return (y + x)**2

print "for h = %f" % 0.1
euler(f, 0.0, 0.0, 1.0, 0.1)
print 
print "for h = %f" % 0.05
euler(f, 0.0, 0.0, 1.0, 0.05)
print
print "Commenting on the order of error, we can easily see that when step size decreases, so does the error."