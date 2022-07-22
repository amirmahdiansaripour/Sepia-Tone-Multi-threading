import pandas as pds
import matplotlib.pyplot as plt
import numpy as np
def showFrequencies():
    dataFrame = pds.read_csv("optimized-number-of-threads.csv")
    dataFrame.hist()
    plt.title('Frequency of optimized number of threads')
    plt.show()
showFrequencies()