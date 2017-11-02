from scipy import stats
import matplotlib.pyplot as plt
import numpy as np

data = np.genfromtxt(open("linearData.dat",'r'))
slope, intercept, r_value, p_value, std_err = stats.linregress(data[:,0],data[:,1])

print("Slope = %.5f" % slope)
print("Intercept = %.5f" % intercept)
print("R-Value = %.5f" % r_value)
print("p-Value = %.5f" % p_value)
print("Standard Error = %.5f" % std_err)

X = np.linspace(min(data[:,0]),max(data[:,0]),20)
Y = slope*X + intercept

plt.title("Linear Data")
plt.xlabel('x')
plt.ylabel('y')
plt.plot(data[:,0],data[:,1],'bo',X,Y,'r-')
plt.grid()
plt.show()