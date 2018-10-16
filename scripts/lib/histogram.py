# necessary
import matplotlib as mpl
mpl.use('agg')
import matplotlib.pyplot as plt
from os import makedirs, listdir
from os.path import exists
import numpy as np

# for main
import pandas as pd
import sys

class Histogram:
    # initial data
    runName = None
    values = None
    n_bins = None
    # plot details
    xlabel = None
    ylabel = None
    plottitle = None

    # histogram details
    entries = None
    bins = None
    ranges = None
    mean = None
    std = None
    fig = None

    def __init__(self, val, bins, run_name, x_label = '' , title = None, y_label = 'Counts'):
        # val - Values: values to plot on histogram
        # bins - Number of Bins
        # run_name - Name of Run: e.g. RandomWalk, 1HRandomWalk
        # x_label, y_label, title - For plots

        # set the variables we know
        self.runName = run_name
        self.values = val
        self.n_bins = bins
        self.xlabel = x_label
        self.ylabel = y_label
        self.plottitle = title
        self.plotHist()

    def plotHist(self):
        # make histogram
        self.entries = len(self.values)
        self.fig, ax = plt.subplots()
        self.bins, self.ranges, self.patches = ax.hist(self.values, self.n_bins)

        # calc mean and std
        Px = 0
        Px2 = 0

        for quant, lower, upper in zip(self.bins, self.ranges[:-1], self.ranges[1:]):
            middle_value = (upper + lower) / 2
            Px += quant * middle_value
            Px2 += quant * (middle_value ** 2)
        self.mean = Px / self.entries
        self.std = np.sqrt(Px2 / self.entries - (self.mean) ** 2)

        # save title and labels
        if not self.plottitle is None:
            ax.set_title('%s\nMean: %.4e, Std: %.4e | Entries: %i' % (self.plottitle, self.mean, self.std, self.entries))
        else:
            ax.set_title('Mean: %.4e, Std: %.4e | Entries: %i' % (self.mean, self.std, self.entries))

        ax.set_xlabel(self.xlabel)
        ax.set_ylabel(self.ylabel)

    def saveHistogram(self, filename = None):
        # default is runName
        if filename is None:
            filename = self.runName

        # better safe than sorry
        if not exists('plots'):
            makedirs('plots')
        if not exists('plots/histograms'):
            makedirs('plots/histograms')
        if not exists('plots/histograms/%s' % self.runName):
            makedirs('plots/histograms/%s' % self.runName)
        # save fig
        self.fig.savefig('plots/histograms/%s/%s.png' % (self.runName, filename))
        print('Histogram png saved at: plots/histograms/%s/%s.png' % (self.runName, filename))

    def saveFile(self, filename = None):
        # default is runName
        if filename is None:
            filename = self.runName

        # better safe than sorry
        if not exists('data_files'):
            makedirs('data_files')
        if not exists('data_files/histograms'):
            makedirs('data_files/histograms')
        if not exists('data_files/histograms/%s' % self.runName):
            makedirs('data_files/histograms/%s' % self.runName)

        # open file to save to
        with open('data_files/histograms/%s/%s.hist' % (self.runName, filename), 'w') as histfile:
            # going to store all necessary variables in a text file
            histfile.write('# hashes denote comments, these lines are ignored\n')
            histfile.write('# note .hist files do not store raw data\n')
            histfile.write('run-name:%s\n' % self.runName)
            histfile.write('nbins:%i\n' % self.n_bins)
            histfile.write('x-label:%s\n' % self.xlabel)
            histfile.write('y-label:%s\n' % self.ylabel)
            histfile.write('plot-title:%s\n' % self.plottitle)
            histfile.write('entries:%i\n' % self.entries)
            histfile.write('mean:%.4e\n' % self.mean)
            histfile.write('std:%.4e\n' % self.std)
            histfile.write('bins:%s\n' % ('%s' % (tuple(self.bins), ))[1:-1]) # ignores brackets, just for easier loading
            histfile.write('ranges:%s\n' % ('%s' % (tuple(self.ranges), ))[1:-1])

    def loadFile(self, filename):
        with open('data_files/histograms/%s.hist' % (filename), 'r') as histfile:
            for num, line in enumerate(histfile):
                # ignore comment lines
                if line.strip()[0] == '#':
                    continue

                # split line at the colon
                line = line.split(':')

                if not len(line) == 2:
                    print('Line %i in %s: poorly formatted line (more than one colon)\n%s' % (num, filename, line))
                    continue

                key = line[0]
                value = line[1]
                try:
                    # check field and value
                    if key == 'run-name':
                        self.runName = value
                    if key == 'nbins':
                        self.n_bins = int(value)
                    if key == 'x-label':
                        self.xlabel = value
                    if key == 'y-label':
                        self.ylabel = value
                    if key == 'plot-title':
                        self.plottitle = value
                    if key == 'entries':
                        self.entries = int(value)
                    if key == 'mean':
                        self.mean = float(value)
                    if key == 'std':
                        self.std = float(value)
                    if key == 'bins':
                        self.bins = [float(f) for f in value.split(',')]
                    if key == 'ranges':
                        self.bins = [float(f) for f in value.split(',')]
                except Exception as e:
                    print('Line %i in %s: poorly formatted line\n%s\n%s' % (num, filename, line, str(e)))
                else:
                    print('Loaded %s.' % filename)

class Histogram_fromFile(Histogram):
    def __init__(self, filename):
        self.loadFile(filename)
        #NEED FIXING
        self.plotHist()



def main(file_name = 'testing'):
    datafiles = [file for file in listdir('data_files/csvs') if file.split('_')[0] == file_name]
    print(len(datafiles))
    data = []
    #data_df = pd.DataFrame()

    for i, datafile in enumerate(datafiles):
        if i % 50 == 0:
            print(i)
        dat = pd.read_csv('data_files/csvs/%s' % datafile, sep = '\t', index_col = 0)
        filter_dat = dat.loc[(dat['Team'] == 'Home') & (dat['Num'] == 1) & (dat['FID'] != 0)]
        data += filter_dat['dCtrl'].values.tolist()
        #data_df = data_df.append(dat, ignore_index=True)

    hist = histogram()
    # if not exists('data_summaries'):
    #     makedirs('data_summaries')

    #data_df.to_csv('data_files/csvs/%s.csv' % file_name, sep = '\t')

if __name__ == '__main__':
    if len(sys.argv) == 2:
        file_name = sys.argv[1]
    else:
        print('''
    %s - plots histograms of dCtrl for shirt 1 on the home team
    args:
        [1] - filename (name of csv in data_files/filename_#.csv)
    ''')
        exit()

    main(file_name)
