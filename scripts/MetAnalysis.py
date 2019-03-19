import numpy as np 
import matplotlib as mpl 
mpl.use('agg')
import matplotlib.pyplot as plt
import pandas as pd
from scipy import stats
import sys
from os.path import exists
from os import listdir, makedirs
import time


class Histogram:
    values = None
    num_entries = None
    num_bins = None
    bin_width = None
    data = None
    mean = None
    stdev = None

    plot = None

    # data is a list
    def __init__(self, data):
        self.values = data
        self.num_entries = len(self.values)

        # use the Freedman-Diaconis rule for number of bins
        h = 2 * stats.iqr(self.values) * (self.num_entries ** (-1/3))
        #print(h)
        #print(max(self.values) - min(self.values))
        self.num_bins = int((max(self.values) - min(self.values)) / h)
        #print(self.num_bins)
        bins, ranges = np.histogram(self.values, self.num_bins)

        middles = []
        for lower, upper in zip( ranges[:-1], ranges[1:]):
            middles.append((upper + lower) / 2)
        self.data = list(zip(middles, bins))
        self.bin_width = middles[1] - middles[0]

        # calculate mean and standard deviation
        Px = 0; Px2 = 0
        for m, b in self.data:
            Px += b * m
            Px2 += b * (m ** 2)
        self.mean = Px / self.num_entries
        self.stdev = np.sqrt(Px2 / self.num_entries - (self.mean) ** 2)



    def PlotHistogram(self, path, filename, title, xlabel, ylabel = 'Counts'):
        # plots histogram
        x, y = zip(*self.data)
        self.plot = plt.bar(x, y, width = self.bin_width, align = 'center', color = '#1f77b4', edgecolor = '#1f77b4')
        
        # formatting
        plt.ylim(0,)
        plt.xlabel(xlabel)
        plt.ylabel(ylabel)
        plt.title('%s\nMean: %.4e | Std: %.4e | Entries: %i | Bins: %i' % (title, self.mean, self.stdev, self.num_entries, self.num_bins))
        
        if not exists('%s' % path):
            makedirs('%s' % path)
        plt.savefig('%s/%s.png' % (path, filename))
        print('Histogram saved at: %s/%s.png' % (path, filename))
        plt.clf()


# load files into dataframe
def LoadFiles(filename):
    path = 'data_files/csvs/%s' % filename
    datafiles = [file for file in listdir(path) if file.split('.')[-1] == 'met']    
    
    data = pd.DataFrame(columns=('Ally', 'ShirtNum', 'Distance'))
    #print(data.shape)

    for i in range(len(datafiles)):
        df = pd.read_csv('%s/%i.met' % (path, i))
        df.columns = ['Ally', 'ShirtNum', 'Distance']
        #print(df)
        data = data.append(df)
    
    return data


def CalcAllyRatio(df):
    players = [int(x) for x in df['Ally'].tolist()]

    num_players = len(players)
    num_allies = players.count(1)
    num_opponents = players.count(0)

    ally_ratio = num_allies / num_players
    opp_ratio = num_opponents / num_players
    print('Number of entries: %i' % num_players) 
    print('Proportion of allies: %.1f%%' % (100 * ally_ratio))
    print('Proportion of opponents: %.1f%%' % (100 * opp_ratio))
    return ally_ratio, opp_ratio


def CalcMeanDist(df):
    distances = [float(d) for d in df['Distance'].tolist()]
    mean_dist = sum(distances)/len(distances)

    print('Mean distance of players considered: %.1fm' % (mean_dist))
    return mean_dist


def main(filename):
    # get start time
    start_time = time.time()

    # load data
    df = LoadFiles(filename)
    ally_ratio, opp_ratio = CalcAllyRatio(df)
    mean_dist = CalcMeanDist(df)
    dist_data = list(df['Distance'])
    
    # plot for all players
    path = 'plots/histograms/%s' % filename
    title = 'Distance to considered players in MetricN'
    xlabel = 'Distance'; ylabel = 'Counts'
    hist = Histogram(dist_data)
    hist.PlotHistogram(path, title, title, xlabel, ylabel)

    # for allies
    dist_data = list(df.loc[df['Ally'] == 1, 'Distance'])
    hist = Histogram(dist_data)
    hist.PlotHistogram(path, '%s-Allies' % title, '%s: Allies' % title, xlabel, ylabel)
    
    # for opponents
    dist_data = list(df.loc[df['Ally'] == 0, 'Distance'])
    hist = Histogram(dist_data)
    hist.PlotHistogram(path, '%s-Opponents' % title, '%s: Opponents' % title, xlabel, ylabel)

    time_elapsed = time.time() - start_time
    print('Time taken: %.2fs' % time_elapsed)

if __name__ == '__main__':
    # sys args
    if len(sys.argv) == 2:
        filename = sys.argv[1]

    else:
        print('''
        %s - plots histograms of distance data from .met files.
        Args:
        [1] - filename (DD.MM.Match:HomeTeam:AwayTeam)
        ''' % sys.argv[0])
        exit()

    main(filename)