# Load a set of matches from data_files/csvs in the form filename_#.csv.
# The matches are then filtered by a set of criteria. These can be expressed
# as functions which take the Pandas DataFrame as an argument and
# filtered DataFrame.
#

# necessary
import pandas as pd
import numpy as np
import matplotlib as mpl
mpl.use('agg')
import matplotlib.pyplot as plt
from os import makedirs, listdir
from os.path import exists
import lib.DFFilters as DFF
from lib.DFFilters import FilterData
from lib.histogram import Histogram, Histogram_fromFile
# for main
import sys

def LoadRunName(run_name):
    datafiles = [file for file in listdir('data_files/csvs') if file.split('_')[0] == run_name]
    num_files = len(datafiles)
    print('%s: %i csv files found' % (run_name, num_files))

    data_df = pd.DataFrame()
    for i, datafile in enumerate(datafiles):
        # print every 10%
        if num_files > 10 and i % (num_files // 10) == 0:
            print('%i%%: loading %s' % (int(100 * i / num_files), datafile))
        # load csv
        dat = pd.read_csv('data_files/csvs/%s' % datafile, sep = '\t', index_col = 0)
        # append to dataframe
        data_df = data_df.append(dat, ignore_index = True)

    print('100%: finished.')
    return(data_df)

def FilterSmartWalker(dataframe):
    # apply selection cuts here

    # select smart player
    dataframe, diff1 = FilterData(dataframe, DFF.SlSmartPlayer)
    print('\tSmartPlyr cut: %i' % diff1)

    # remove frames when the player is stationary
    dataframe, diff2 = FilterData(dataframe, DFF. RmStationary)
    print('\tStationary cut: %i' % diff2)

    # remove frame 0
    dataframe, diff3 = FilterData(dataframe, DFF.Rm0Frame)
    print('\tFrame cut: %i' % diff3)

    return(dataframe)

def AnalyseSim(run_name, bins, filename = None, title = None, fltr = FilterSmartWalker, x_label = 'dCtrl [Ctrl(t_n) - Ctrl(t_n - 1)]', y_label = 'Counts'):
    # check args
    if filename is None:
        filename = run_name
    if title is None:
        title = filename

    # # gather data
    data = LoadRunName(run_name)

    print('\nApplying cuts')
    # apply cuts
    data = fltr(data)
    print('\n')

    # select dCtrl
    data = data['dCtrl'].values

    # plot histogram
    hist = Histogram(data, bins, run_name, x_label, title, y_label)
    hist.saveHistogram(filename)
    hist.saveFile(filename)
    plt.clf()

    # log plot
    # remove zero bins
    corr = [[b, m] for [b, m] in zip(hist.bins, hist.middle_values) if not b == 0]
    corr_bins = [b for b, m in corr]
    corr_mid = [m for b, m in corr]
    corr_bins = np.log(corr_bins)

    log_plot = plt.scatter(corr_mid, corr_bins, marker = 'x' )
    plt.title('%s - Logarithm Plot' % filename)
    plt.xlabel('dCtrl')
    plt.ylabel('ln(Counts)')
    plt.savefig('plots/histograms/%s/%sLog.png' % (filename, filename))
    plt.clf()

if __name__ == '__main__':
    # process sys args
    if len(sys.argv) == 4:
        run_name = sys.argv[1]
        bins = int(sys.argv[2])
        filename = sys.argv[3]
    else:
        print('''
    %s - Runs the simulation multiple times
        args:
        [1] - Run Type (RandomWalk/1HRandomWalk/1HExchange/UnitSquare)
        [2] - Samples [default: 400]
        [3] - Frames per sample [default: 15000]
        [4] - Filename [default: (Run Type)]
        [5] - Legacy file (yes/no) [default: (no)]
        [6] - Processes [default: 8]
        ''' % sys.argv[0])
        exit()
    # args complete, run main function
    AnalyseSim(run_name, bins, filename)
