import matplotlib.pyplot as plt
import pandas as pds

def drawGraph(x, y, serialTime):
    minThreads, minParallelTime = findOptimizedPoint(x, y)
    plt.figure(figsize=(100, 100))
    plt.plot(x, y, marker='o', color='b')
    
    plt.axhline(y= serialTime, color= 'r')
    plt.text(80, serialTime, "Serial time spent: " + "{:.0f}".format(serialTime), color="red", ha="right", va="bottom", fontsize=18)
    
    plt.axhline(y= minParallelTime, color= 'g')
    plt.text(80, minParallelTime, "Min parallel time spent: " + "{:.0f}".format(minParallelTime), color="green", ha="right", va="bottom", fontsize=18)
    
    plt.xlabel("Number of threads")
    plt.ylabel("Time (ms)")
    plt.figtext(0.5, 0.01, "Optimized number of threads is " + str(minThreads), horizontalalignment = 'center', fontsize = 20)
    plt.show()
    

def findOptimizedPoint(x, y):
    minThread = x[0]
    minTime = y[0]
    for i in range(len(x)):
        if(y[i] < minTime):
            minThread = x[i]
            minTime = y[i]
    return minThread, minTime


def plotRates():
    dataFrame = pds.read_csv("../samples.csv")
    numberOfRows = dataFrame.shape[0] - 1
    x = dataFrame['x'][0:numberOfRows]
    y = dataFrame['y'][0:numberOfRows]
    serialTimeSpent = dataFrame['x'][numberOfRows]
    drawGraph(x, y, serialTimeSpent)
    return

plotRates()