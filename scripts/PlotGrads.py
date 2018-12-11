import numpy as np 
import matplotlib as mpl 
mpl.use('agg')
import matplotlib.pyplot as plt
import pandas as pd
import sys
from PeakFitting import Histogram
from os.path import listdir, makedirs

def LoadGrads(filename):
    datafiles = [file for file in listdir('plots/grad_files/%s' % filename) if file.split('.')[-1] == 'grd']
    
    df = pd.DataFrame(columns = ['Home', 'Away'])
    homedata = []
    awaydata = []
    
    for file in datafiles:
        if file.split(' ')[-2] == 'Home':
                homedata.extend(file)
        if file.split(' ')[-2] == 'Away':
            awaydata.extend(file)
    
    # add data to dataframe
    df['Home'] = pd.Series(homedata).values
    df['Away'] = pd.Series(awaydata).values

    return(df)

def PlotGrads(df, title, xlabel, ylabel, filename):
    homehist = Histogram(df, 'Home')
    awayhist = Histogram(df, 'Away')

    # histograms save to 'plots/histograms/filename/title | team.png'
    homehist.plotHistogram(homehist.data, '%s | Home' % title, xlabel, ylabel, filename, True)
    awayhist.plotHistogram(awayhist.data, '%s | Away' % title, xlabel, ylabel, filename, True)

def main(filename):
    # plot formatting
    title = filename
    xlabel = 'Mean dCtrl'
    ylabel = 'Counts'

    # plot histograms
    df = LoadGrads(filename)
    PlotGrads(df, title, xlabel, ylabel, filename)

if __name__ == '__main__':
    # process sys args
    if len(sys.argv) == 2:
        filename = sys.argv[1]
    else:
        print(
        '''
        %s - Plots histograms of mean dCtrl over possession regions of matches.
        Args:
        [1] - Filename
        ''' % sys.argv[0])
        exit()
