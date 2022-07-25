import pandas as pds
import matplotlib.pyplot as plt
import numpy as np

outLierRatio = 0.005

def removeOutLiers(dataFrame):
    numberOfrows = dataFrame.shape[0] + 1
    data = dataFrame.copy(deep=True)
    uniqueValues = data.iloc[:,0].unique()
    keys = []
    values = []
    for value in uniqueValues:
        if((data[data.iloc[:,0] == value].shape[0] + 1) < numberOfrows * outLierRatio):
            data.drop(data[data.iloc[:,0] == value].index, inplace = True)
        else:
            keys.append(value)
            values.append(data[data.iloc[:,0] == value].shape[0] + 1)
    return keys, values    


def showFrequencies():
    dataFrame = pds.read_csv("optimized-number-of-threads.csv")
    keys, values = removeOutLiers(dataFrame)
    plt.figure(figsize=(100, 100))
    plt.bar(keys, values,width = 0.4)
    # print(dataFrame.iloc[:,0].value_counts())
    plt.title('Frequency of optimized number of threads')
    plt.show()
showFrequencies()