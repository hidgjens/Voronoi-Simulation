import pandas as pd
import numpy as np
import matplotlib as mpl
mpl.use('agg')
import sys
import matplotlib.pyplot as plt
from os import makedirs, listdir
from os.path import exists
import scripts.lib.DFFilters as DFF
from scripts.lib.DFFilters import FilterData
from scipy.signal import find_peaks
from lmfit import Model

def LoadRunName(filename):
    datafiles = [file for file in listdir('data_files/csvs') if file.split('_')[0] == filename]
    num_files = len(datafiles)
    print('%s: %i csv files found' % (filename, num_files))

    df = pd.DataFrame()
    for i, datafile in enumerate(datafiles):
        # print every 10%
        if num_files > 10 and i % (num_files // 10) == 0:
            print('%i%%: loading %s' % (int(100 * i / num_files), datafile))
        # load csv
        dat = pd.read_csv('data_files/csvs/%s' % datafile, sep = '\t', index_col = 0)
        # append to dataframe
        df = df.append(dat, ignore_index = True)

    print('100%: finished.')
    return(df)

def getFilterSettings(run_name):
    # format of filter settings (all values are bools):
    # [RemoveHomeTeam, RemoveAwayTeam, RemoveBalls, SelectSmartPlayer, RemoveStationary]
    filter_settings = {
    'RandomWalk': [False, False, True, False, False],
    '1HExchange': [False, True, True, True, False],
    'RandomWalkers:11:RandomWalkers:0': [False, True, True, False, False],
    'MetricTeam:11:RandomWalkers:0': [False, True, True, False, False],
    'MetricTeam:11:RandomWalkers:11': [False, True, True, False, False],
    'MetricTeam:11:MetricTeam:0' : [False, True, True, False, False],
    'ExchangeTeam:11:ExchangeTeam:0' : [False, True, True, False, False],
    'MetricSim' : [False, True, True, False, False]
    }

    if len(run_name.split(':')) == 3:
        return(filter_settings['MetricSim'])
    else:
        return(filter_settings[run_name])

def filter(df, run_name):
    # apply filter settings
    filters = ['RmHomeTeam', 'RmAwayTeam', 'RmBall', 'SlSmartPlayer', 'RmStationary']
    filter_settings = getFilterSettings(run_name)
    filt_dict = dict(zip(filters,filter_settings))

    # remove frame 0 as default
    df, diff1 = FilterData(df, DFF.Rm0Frame)
    print('\tRm0Frame cut: %i' % diff1)

    # other filters applied based on settings
    for filt, setting in filt_dict.items():
        if setting:
            filtered, diff = FilterData(df, getattr(DFF, filt))
            print('\t%s cut: %i' % (filt, diff))
            df = filtered

    return(df)
    
def peakDetect(data):
    # detects peaks in histogram data
    # unzip data
    x, y = zip(*data)
    del x
    # prominence gives minimum counts that peak must stand out from background by
    # width sets minimum width of peak (in samples)
    peaks, properties = find_peaks(y, prominence = 1000)
    print(properties)

    return(peaks)

def gaussianPeakFit(data, peak):
    # fits Gaussian to peak in histogram data
    # define Gaussian function to fit
    def gaussian(x, A, mu, sig):
        return (A / (np.sqrt(2*np.pi) * sig)) * np.exp(-(x-mu)**2 / (2*sig**2))

    # select regions around peak
    lower = peak - 100
    upper = peak + 100
    if lower < 0:
        lower = 0
    if upper > len(data):
        upper = len(data)
    
    # get corresponding data
    x, y = zip(*data[lower:upper]) 

    # get x and y co-ordinate of peak
    px, py = data[peak]        

    # fit Gaussian to peak region
    gmodel = Model(gaussian)
    gfit = gmodel.fit(y, x = x, A = py, mu = px, sig = 1 / np.sqrt(py))

    return(gfit)

class Histogram:
    values = None
    n = None
    nbins = None
    data = None
    binwidth = None
    mean = None
    stdev = None

    def __init__(self, df, var):
        # entries
        self.values = df[var].values
        self.n = len(self.values)
        
        # bin number
        scale = 500
        self.nbins = int(scale * (self.values.max()-self.values.min())) 

        # compute histogram data
        fig, ax = plt.subplots()
        bins, ranges, patches = ax.hist(self.values, self.nbins)
        del patches
        del fig

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

    def quadFitPlot(self, data, title, xlabel, ylabel, filename, save):
        # computes and plots quadratic fit on a histogram
        plot = self.plotHistogram(data, title, xlabel, ylabel, filename, save = False)

        # quadratic fit
        x, y = zip(*data)
        qfit = np.polyfit(x, y, 2)
        f = np.poly1d(qfit)
        xfit = np.linspace(x[0], x[-1], 100)
        yfit = f(xfit)
        plt.plot(xfit, yfit, linestyle = '-', color = 'r')

        # formatting equation of fit
        signs = []
        for q in qfit:
            sign = np.sign(q)
            if sign >= 0:
                signs.append('+')
            else:
                signs.append('')

        # display equation of fit
        plt.text(min(x), max(y), '$y = %.2fx^{2} %s %.2fx %s %.2f$' % (qfit[2],signs[1],qfit[1],signs[0],qfit[0]))

        # formatting
        plt.ylim(0,)

        if save:
            plt.savefig('plots/histograms/%s/%s.png' % (filename, title))
            print('Histogram saved at: plots/histograms/%s/%s.png' % (filename, title))
            plt.clf()
        else:
            return(plot)

    def plotHistogram(self, data, title, xlabel, ylabel, filename, save):
        # plots histogram of loaded data
        x, y = zip(*data)
        plot = plt.bar(x, y, width = self.binwidth, align = 'center', color = '#1f77b4', edgecolor = '#1f77b4')

        # formatting
        plt.ylim(0,)
        plt.xlabel(xlabel)
        plt.ylabel(ylabel)

        plt.title('%s\nMean: %.4e | Std: %.4e | Entries: %i | Bins: %i' % (title, self.mean, self.stdev, self.n, self.nbins))
        
        if save:
            plt.savefig('plots/histograms/%s/%s.png' % (filename, title))
            print('Histogram saved at: plots/histograms/%s/%s.png' % (filename, title))
            plt.clf()
        else:
            return(plot)
    
    def computeLog(self):
        # computes natural logarithm of a given set of histogram data
        # remove zero bins, take log of remaining bins, return corrected bins
        corr = [[b, m] for [b, m] in self.data if not b == 0]
        y = np.log([b for b, m in corr])
        x = [m for b, m in corr]

        return(zip(x,y))

    def gaussianPeakPlot(self, data, title, xlabel, ylabel, filename, save):
        # plots Gaussian fits to peaks in a histogram
        plot = self.plotHistogram(data, title, xlabel, ylabel, filename, save = False)
        peaks = peakDetect(data)
        print(peaks)
        for peak in peaks:
            print('Peak: %i' % peak)
            # select regions around peak
            lower = peak - 100
            upper = peak + 100
            if lower < 0:
                lower = 0
            if upper > len(data):
                upper = len(data)
        
            # get corresponding data
            x, y = zip(*data[lower:upper]) 
            del y
            
            # plot Gaussian fit
            # xfit =np.linspace(x[0], x[-1], 500)
            gfit = gaussianPeakFit(data, peak)
            plt.plot(x, gfit.best_fit, 'r-')
            print(gfit.fit_report())
            
        if save:
            plt.savefig('plots/histograms/%s/%s - Gaussian Fit.png' % (filename, title))
            print('Histogram saved at: plots/histograms/%s/%s - Gaussian Fit.png' % (filename, title))
            plt.clf()
        else:
            return(plot)
    
# test main function to check Gaussian fitting is working            
def main():
    date = '31.10'
    run_name = 'MetricTeam:11:RandomWalkers:11'
    filename = '%s.%s' % (date, run_name) 
    # load and filter data
    df = filter(LoadRunName(filename), run_name)
    hist = Histogram(df, 'dCtrl')
    title = '%s | Gaussian Peak Fitting Test' % filename
    hist.gaussianPeakPlot(hist.data, title, 'dCtrl', 'Counts', filename, True)

if __name__ == '__main__':
    print('Starting:\n')
    main()

