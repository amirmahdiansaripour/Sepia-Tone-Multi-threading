import matplotlib.pyplot as plt
import pandas as pds
def plotRates():
    dataFrame = pds.read_csv("samples.csv")
    x = dataFrame['x']
    y = dataFrame['y']
    plt.plot(x, y)
    plt.show()
    # print(x)
plotRates()