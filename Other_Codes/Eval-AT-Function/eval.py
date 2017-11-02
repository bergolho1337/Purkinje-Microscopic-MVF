import sys
import numpy as np

def func (lsize,lcell,alpha,d):
	return np.exp( 8.63393299 * alpha - 0.10125643 * d ) + (2.81811174 * lsize + 3.81821235 * lcell) + 5.44446901

def delay(alpha,d):
    return np.exp( 8.63393299 * alpha - 0.10125643 * d ) + 5.45857374

def seg (lsize,lcell):
    return 2.81811174 + (3.81821235 * lsize) - (0.01410473 * lcell)    		

lsize =  float(sys.argv[1])
lcell =  float(sys.argv[2])
alpha =  float(sys.argv[3])
d =  float(sys.argv[4])

print("lsize = %.5f" % lsize)
print("lcell = %.5f" % lcell)
print("alpha = %.5f" % alpha)
print("d1 = %.5f" % d)
print("Seg = %.5f" % seg(lsize,lcell))
print("Delay = %.5f" % delay(alpha,d))
print("AT-Prediction = %.5f" % (seg(lsize,lcell) + delay(alpha,d)) )
