import matplotlib.pyplot as plt
import pandas as pds
def plotRates():
    dataFrame = pds.read_csv("samples.csv")
    x = dataFrame['x']
    y = dataFrame['y']
    plt.plot(x, y, marker='o', color='b')
    plt.xlabel("Number of threads")
    plt.ylabel("Time (ms)")
    plt.show()
    plt.close()
    # print(x)
plotRates()