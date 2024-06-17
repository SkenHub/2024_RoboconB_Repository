import matplotlib.pyplot as plt
import numpy as np

g = 9.8065
R = 0.3
rps = 6

def f1(t):
    return 0.5*g*(t**2)

def f2(t,omega):
    return R-R*np.cos(omega*t)

if __name__ == "__main__":
    t = np.linspace(0,2.0*np.sqrt(R/g),100)
    omega = rps*2*np.pi#(np.pi/2.0)*np.sqrt(g/R)*1.5
    x1 = []
    x2 = []
    for i in range(len(t)):
        x1.append(f1(t[i]))
        x2.append(f2(t[i],omega))
    print(omega/(2*np.pi))
    
    plt.grid()
    plt.plot(t,x1)
    plt.plot(t,x2)
    plt.show()