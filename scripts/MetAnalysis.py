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

def LoadList(filename):
    path = 'data_files/csvs/%s' % filename
    datafiles = [file for file in listdir(path) if file.split('.')[-1] == 'met']

    return_data = []

    for d in datafiles:
        with open('%s/%s' % (path, d), 'r') as dfile:
            # raw_data = np.loadtxt(dfile)
            ally_player = []

            for line in dfile.readlines():
                split_line = line.split(',')
                ally_player.append(float(split_line[0].strip()))

            # frmt_ally_player = [float(f) for f in ally_player]
            return_data.extend(ally_player)

    return return_data



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

def breaklistup(N, list_to_split):
    return_list = []

    len_of_list = len(list_to_split)
    # print(len_of_list)

    for i in range(0,len_of_list,N):
        # print(i)
        sublist = []
        for j in range(0, N):
            idx = i + j
            if (idx < 0):
                print('p r a n g')
                exit()
            # print('%i/%i' % (idx, len_of_list))
            sublist.append(list_to_split[idx])

        return_list.append(sublist)

    # print(return_list)

    return return_list

def CalcAllyRatioFromList(l):
    N = len(l)
    
    ratio_of_allies = sum(l) / N
    return(ratio_of_allies)

def CalcMeanFromListofList(lol):
    samples = len(lol)

    sum_of_x = 0
    sum_of_x_squared_above = 0
    sum_of_x_squared_below = 0
    samples_above = 0
    samples_below = 0

    for l in lol:
        x = CalcAllyRatioFromList(l)
        sum_of_x += x

    mean_of_x = sum_of_x / samples
        
    for l in lol:
        x = CalcAllyRatioFromList(l)
        if x > mean_of_x:
            sum_of_x_squared_above += x ** 2
            samples_above += 1
        else:
            sum_of_x_squared_below += x ** 2
            samples_below += 1
        

    variance_of_x_above = sum_of_x_squared_above / samples_above - (mean_of_x ** 2)
    variance_of_x_below = sum_of_x_squared_below / samples_below - (mean_of_x ** 2)

    std_of_x_above = np.sqrt(variance_of_x_above)
    std_of_x_below = np.sqrt(variance_of_x_below)

    return(mean_of_x, [std_of_x_above, std_of_x_below])

def LauncherAnalysis(N, filename, df):
    big_list = df
    lol = breaklistup(N, big_list)
    mean, stds = CalcMeanFromListofList(lol)

    print('N: %i | Mean: %.3f - Std: +%.3f/-%.3f' % (N, mean, *stds))

    if not exists('results/MetricN/%s' % filename):
        makedirs('results/MetricN/%s' % filename)

    with open('results/MetricN/%s/%s.mnr' % (filename, filename), 'w') as result_file:
        result_file.write('%.4f\n%.4f\n%.4f\n' % (mean, *stds))


def CalcMeanDist(df):
    distances = [float(d) for d in df['Distance'].tolist()]
    mean_dist = sum(distances)/len(distances)

    print('Mean distance of players considered: %.1fm' % (mean_dist))
    return mean_dist

  

def main(filename, N):
    # get start time
    start_time = time.time()

    # # load data
    # df = LoadFiles(filename)

    # print(df)

    df = LoadList(filename)

    print(len(df))

    LauncherAnalysis(N, filename, df)





    # ally_ratio, opp_ratio = CalcAllyRatio(df)
    # mean_dist = CalcMeanDist(df)
    # dist_data = list(df['Distance'])

    # # save results to text file
    
    # savepath = 'results/metricN/%s' % filename
    # if not exists(savepath):
    #     makedirs(savepath)

    # # f = open('%s/%s.mnr' % (savepath, filename), 'w')
    # # f.write('# Lines are: 1) Ally ratio 2) Opponent ratio 3) Mean distance to players\n')
    # # f.write('%f\n%f\n%f\n' % (ally_ratio, opp_ratio, mean_dist))
    # # f.close()
    # # print('Results saved at: %s/%s.mnr' % (savepath, filename))

    # # plot for all players
    # path = 'plots/histograms/%s' % filename
    # title = 'Distance to considered players in MetricN'
    # xlabel = 'Distance'; ylabel = 'Counts'
    # hist = Histogram(dist_data)
    # hist.PlotHistogram(path, title, title, xlabel, ylabel)

    # # for allies
    # dist_data = list(df.loc[df['Ally'] == 1, 'Distance'])
    # hist = Histogram(dist_data)
    # hist.PlotHistogram(path, '%s-Allies' % title, '%s: Allies' % title, xlabel, ylabel)
    
    # # for opponents
    # dist_data = list(df.loc[df['Ally'] == 0, 'Distance'])
    # hist = Histogram(dist_data)
    # hist.PlotHistogram(path, '%s-Opponents' % title, '%s: Opponents' % title, xlabel, ylabel)

    time_elapsed = time.time() - start_time
    print('Time taken: %.2fs' % time_elapsed)

if __name__ == '__main__':
    # sys args
    if len(sys.argv) == 3:
        filename = sys.argv[1]
        N = int(sys.argv[2])

    else:
        print('''
        %s - plots histograms of distance data from .met files.
        Args:
        [1] - filename (DD.MM.Match:HomeTeam:AwayTeam)
        ''' % sys.argv[0])
        exit()

    main(filename, N)