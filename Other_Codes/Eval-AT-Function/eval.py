from pylab import *
import scipy
import numpy as np

def delay(x1,x2,x3,x4):
    coef = [1.64005959e+03,-1.91835183e+03,3.40996996e-03,1.30797791e+01]
    return coef[0]*exp(coef[1]*x3) + coef[2]*exp(coef[3]*x4)

def seg (x1,x2,x3,x4):
    coef = [3.81768672,-0.01402317,1.42079815,-0.02818996,2.73394343]
    return coef[0]*x1 + coef[1]*x2 + coef[2]*np.sqrt(x3) + coef[3]*x4 + coef[4]    		

def biff (x1,x2,x3,x4,x5):
    coef = [-2.05812575e-03,-6.37789352e-05,1.87178472e-02,-4.36363636e-03,2.19444444e+00,3.15444444e-01,-1.26703852e+00]
    return coef[0]*x1 + coef[1]*x2**2 + coef[2]*x2 + coef[3]*x4 + coef[4]*x3 + coef[5]*x5 + coef[6]

# Prediction function of total activation time (consider the delay of the PMJ)
def pred_AT_total (x1,x2,x3,x4,x5):
    return seg(x1,x2,x3,x4) + biff(x1,x2,x3,x4,x5) + delay(x1,x2,x3,x4)

# Prediction function of Purkinje terminals (NOT considering the delay of the PMJ)
def pred_AT_term (x1,x2,x3,x4,x5):
    return seg(x1,x2,x3,x4) + biff(x1,x2,x3,x4,x5)

def test_AT_Total ():
    print(":> Testing AT Total")
    input_file = open("AT-Total.dat","r")
    mean = 0
    cont = 0        
    for line in input_file:
        words = line.split()
        x1 = float(words[0])
        x2 = float(words[1])
        x3 = float(words[2])
        x4 = float(words[3])
        x5 = float(words[4])
        AT = float(words[5])
        pred = pred_AT_total(x1,x2,x3,x4,x5)
        error = abs(AT-pred)
        mean = mean + error
        cont = cont + 1
        print("(%f %f %f %f %f) = %f --- (Analit = %f) [Error = %f]" % (x1,x2,x3,x4,x5,pred,AT,error))
    print("Mean error = %f" % (mean / float(cont)))

def test_AT_Term ():
    print(":> Testing AT Term")
    input_file = open("AT-Term.dat","r")
    mean = 0
    cont = 0        
    for line in input_file:
        words = line.split()
        x1 = float(words[0])
        x2 = float(words[1])
        x3 = float(words[2])
        x4 = float(words[3])
        x5 = float(words[4])
        AT = float(words[5])
        pred = pred_AT_term(x1,x2,x3,x4,x5)
        error = abs(AT-pred)
        mean = mean + error
        cont = cont + 1
        print("(%f %f %f %f %f) = %f --- (Analit = %f) [Error = %f]" % (x1,x2,x3,x4,x5,pred,AT,error))
    print("Mean error = %f" % (mean / float(cont)))

def main ():
    # Testing the activation of the Purkinje terminals function (the data is from the experiment with bifurcations)
    test_AT_Total()
    test_AT_Term()

if __name__ == "__main__":
    main()

# ------------------------------------------------------------------------------------------
# x1 = lf (length fiber)
# x2 = lc (length cell)
# x3 = d1 (depth cell)
# x4 = alpha
# x5 = #biff (number of bifurcations) 

# TDelay
# coef = []
# Tdelay = 

# TSeg
# coef = [3.81768672,-0.01402317,11.99991418,-0.02819055,2.7752069] 
# Tseg = coef[0]*x1 + coef[1]*x2 + coef[2]*x3 + coef[3]*x4 + coef[4]

# TDelayBiff
# coef = [-2.05812575e-03,-6.37789352e-05,1.87178472e-02,-4.36363636e-03,2.19444444e+00,3.15444444e-01,-1.26703852e+00]
# TDelayBiff = coef[0]*x1 + coef[1]*x2**2 + coef[2]*x2 + coef[3]*x4 + coef[4]*x3 + coef[5]*x5 + coef[6]
# ------------------------------------------------------------------------------------------
