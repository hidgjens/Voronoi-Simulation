import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import sys
from pathlib import Path
from os import makedirs
from os.path import exists

# import seaborn as sns

def openCtrlArray(path, filename):
    df = pd.read_csv('%s/%s.csv' % (path, filename), sep = ',', index_col= False)
    # print(df)
    return df

def plotHeatmap(df, filename):
    # set figure size
    # plt.figure(figsize=(30,20))
    # create heatmap
    # ax = sns.heatmap(df.T, square = True, cmap = 'coolwarm', cbar = True, cbar_kws = {"shrink": 0.5})

    plt.figure()
    ax = plt.gca()
    im = ax.imshow(df.T, cmap = 'coolwarm')

    # formatting
    plt.xticks([])
    plt.yticks([])
    
    # colorbar formatting
    plt.colorbar(im, fraction = 0.03, pad = 0.04, ticks = [0, .25, .5, .75, 1])

    p = Path('.').resolve().parent
    path = '%s/plots/heatmaps' % p

    # check if output folder exists
    if not exists(path):
        makedirs(path)
    
    # save figure
    plt.savefig('%s/%s.png' %(path, filename))
    print('Heatmap saved at plots/heatmaps/%s.png' % filename)

def main(path, filename):    
    plotHeatmap(openCtrlArray(path, filename), filename)

if __name__ == '__main__':
    # default path
    p = Path('.').resolve().parent
    path = '%s/data_files/heatmaps' % p

    # sys args
    if len(sys.argv) == 2:
        filename = sys.argv[1] 
    elif len(sys.argv) == 3:
        filename = sys.argv[1]
        path = sys.argv[2]
    else:
        print('''
        Generates heatmap from csv and saves to file.
        Args:
        [1] - Name of csv to create heatmap from
        [2] - (Optional) Change file path to pull csv from
        ''')

    main(path, filename)