import matplotlib.pyplot as plt
import pandas as pds
def plotRates():
    dataFrame = pds.read_csv("../samples.csv")
    numberOfRows = dataFrame.shape[0] - 1
    min = dataFrame['x'][0]
    x = dataFrame['x'][1:numberOfRows]
    y = dataFrame['y'][1:numberOfRows]
    serialTimeSpent = dataFrame['x'][numberOfRows]
    plt.figure(figsize=(100, 100))
    plt.plot(x, y, marker='o', color='b')
    plt.axhline(y= serialTimeSpent, color= 'r')
    plt.xlabel("Number of threads")
    plt.ylabel("Time (ms)")
    txt="Optimized number of threads is " + str(min)
    plt.figtext(0.5, 0.01, txt, horizontalalignment = 'center', fontsize = 20)
    plt.show()
    # print(x)
plotRates()