import numpy as np
import matplotlib as mpl
import sys
from matplotlib import pyplot as plt
from os import listdir, makedirs 
from typing import List
from os.path import exists


class Histogram:
    values = None
    n = None
    nbins = None
    data = None
    binwidth = None
    mean = None
    stdev = None

    def __init__(self, values:list):
        # entries
        self.values = values
        self.n = len(self.values)
        
        # bin number
        scale = 500
        self.nbins = int(scale * (self.values.max()-self.values.min())) 

        # compute histogram data
        bins, ranges = np.histogram(self.values, self.nbins)

        middles = []
        for lower, upper in zip( ranges[:-1], ranges[1:]):
            middles.append((upper + lower) / 2)
        self.data = list(zip(middles, bins))
        self.binwidth = middles[1] - middles[0]

        # calculate mean and standard deviation
        Px = 0
        Px2 = 0

        for m, b in self.data:
            Px += b * m
            Px2 += b * (m ** 2)
        self.mean = Px / self.n
        self.stdev = np.sqrt(Px2 / self.n - (self.mean) ** 2)


    def PlotHistogram(self, title:str, xlabel:str, ylabel:str, filename:str):
        # plots histogram of loaded data
        x, y = zip(*self.data)
        plt.bar(x, y, width = self.binwidth, align = 'center', color = '#1f77b4', edgecolor = '#1f77b4')
        
        # formatting
        plt.ylim(0,)
        plt.xlabel(xlabel)
        plt.ylabel(ylabel)
        plt.title('%s\nMean: %.4e | Std: %.4e | Entries: %i | Bins: %i' % (title, self.mean, self.stdev, self.n, self.nbins))
        
        if not exists('plots/histograms/%s' % filename):
            makedirs('plots/histograms/%s' % filename)

        plt.savefig('plots/histograms/%s/%s.png' % (filename, title))
        print('Histogram saved at: plots/histograms/%s/%s.png' % (filename, title))
        plt.clf()


    def SaveResults(self, filename:str, team:str):
        path:str = 'results/summaries/%s/%s.sum' % (filename, team)
        results = [self.mean, self.stdev]        
        np.savetxt(path, results)            


def LoadMatches(filename:str):
    datafiles:List[str] = [file for file in listdir('data_files/csvs/%s' % filename) if file.split('.')[-1] == 'csv']
    num_files:int = len(datafiles)
    print('%s: %i csv files found' % (filename, num_files))
    data:np.ndarray = np.empty()
    
    for i, datafile in enumerate(datafiles):
        dat = np.loadtxt(datafile, dtype=float)
        if i == 0:
            data = dat    
        else:
            data.append(dat)

    print('%s: %i csv files loaded' % (filename, num_files))
    return(data)


def main(date:str, run_name:str):
    filename:str = '%s.%s' % (date, run_name)
    data:np.ndarray = LoadMatches(filename)
    homedata:List[float] = data[:,0]
    awaydata:List[float] = data[:,1]

    # create histograms
    homehist = Histogram(homedata)
    awayhist = Histogram(awaydata)

    # save histograms and results
    homehist.PlotHistogram('%s | Home' % filename, 'Team Spatial Control', 'Counts', filename)
    homehist.SaveResults(filename, 'Home')

    awayhist.PlotHistogram('%s | Away' % filename, 'Team Spatial Control', 'Counts', filename)
    awayhist.SaveResults(filename, 'Away')


if __name__ == '__main__':
    # process sys args
    if len(sys.argv) == 3:
        run_name:str = sys.argv[1]
        date:str = sys.argv[2]

    else:
        print('''
    %s - Plots histograms of mean spatial control per match
        Args:
        [1] - Run Name (MatchType:HomeTeam:AwayTeam)
        [2] - Date (DD.MM)
        ''' % sys.argv[0])
        exit()

    # args complete, run main function
    main(date, run_name)