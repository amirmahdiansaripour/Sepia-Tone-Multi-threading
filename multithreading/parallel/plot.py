import matplotlib.pyplot as plt
import pandas as pds
def plotRates():
    dataFrame = pds.read_csv("samples.csv")
    min = dataFrame['x'][0]
    x = dataFrame['x'][1:]
    y = dataFrame['y'][1:]
    plt.figure(figsize=(100, 100))
    plt.plot(x, y, marker='o', color='b')
    plt.xlabel("Number of threads")
    plt.ylabel("Time (ms)")
    txt="Optimized number of threads is " + str(min)
    plt.figtext(0.5, 0.01, txt, horizontalalignment = 'center', fontsize = 20)
    plt.show()
    # print(x)
plotRates()