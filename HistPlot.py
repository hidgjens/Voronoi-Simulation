import pandas as pd
import numpy as np
import matplotlib as mpl
mpl.use('agg')
import matplotlib.pyplot as plt
from os import makedirs, listdir
from os.path import exists
import scripts.lib.DFFilters as DFF
from scripts.lib.DFFilters import FilterData
import sys

def LoadRunName(filename):
    datafiles = [file for file in listdir('data_files/csvs') if file.split('_')[0] == filename]
    num_files = len(datafiles)
    print('%s: %i csv files found' % (filename, num_files))

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

def getFilterSettings(run_name):
    # format of filter settings (all values are bools):
    # [RemoveHomeTeam, RemoveAwayTeam, RemoveBalls, SelectSmartPlayer, RemoveStationary]
    filter_settings = {
    'RandomWalk': [False, False, True, False, False],
    '1HExchange': [False, True, True, True, False],
    'RandomWalkers:11:RandomWalkers:0': [False, True, True, False, False],
    'MetricTeam:11:RandomWalkers:0': [False, True, True, False, False],
    'MetricTeam:11:RandomWalkers:11': [False, True, True, False, False]
    }

    return(filter_settings['%s' % run_name])

def filter(dataframe, run_name):
    # apply filter settings
    filters = ['RmHomeTeam', 'RmAwayTeam', 'RmBall', 'SlSmartPlayer', 'RmStationary']
    filter_settings = getFilterSettings(run_name)
    filt_dict = dict(zip(filters,filter_settings))

    # remove frame 0 as default
    dataframe, diff1 = FilterData(dataframe, DFF.Rm0Frame)
    print('\tRm0Frame cut: %i' % diff1)

    # other filters applied based on settings
    for filt, setting in filt_dict.items():
        if setting:
            filtered_data, diff = FilterData(dataframe, getattr(DFF, filt))
            print('\t%s cut: %i' % (filt, diff))
            dataframe = filtered_data

    return(dataframe)

# if log == True, also saves log plot
# var is variable to be plotted, eg. 'Ctrl', 'dCtrl'
def plotHistogram(run_name, date, var, log):
    filename = '%s.%s' % (date, run_name)
    # load filtered data
    df = filter(LoadRunName(filename), run_name)
    values = df[var].values
    n = len(values)

    # default bin scaling
    scale = 500
    bin_num = int(scale * (values.max() - values.min()))

    fig, ax = plt.subplots()
    bins, ranges, patches = ax.hist(values, bin_num)

    # calc mean and standard deviation
    Px = 0
    Px2 = 0
    middle_values = []

    for quant, lower, upper in zip(bins, ranges[:-1], ranges[1:]):
        middle_value = (upper + lower) / 2
        middle_values.append(middle_value)
        Px += quant * middle_value
        Px2 += quant * (middle_value ** 2)
    mean = Px / n
    stdev = np.sqrt(Px2 / n - (mean) ** 2)

    # save title and labels
    ax.set_title('%s\nMean: %.4e, Std: %.4e | Entries: %i' % (filename, mean, stdev, n))
    ax.set_xlabel(var)
    ax.set_ylabel('Counts')

    # better safe than sorry
    if not exists('plots'):
        makedirs('plots')
    if not exists('plots/histograms'):
        makedirs('plots/histograms')
    if not exists('plots/histograms/%s' % filename):
        makedirs('plots/histograms/%s' % filename)

    # save fig
    fig.savefig('plots/histograms/%s/%s%s - %i Bins.png' % (filename, filename, var, bin_num))
    plt.clf()
    print('\nHistogram png saved at: plots/histograms/%s/%s%s - %i Bins.png\n' % (filename, filename, var, bin_num))

    # log plot
    if log == True:
        # remove zero bins, take log of remaining bins, output bar plot
        corr = [[b, m] for [b, m] in zip(bins, middle_values) if not b == 0]
        y = np.log([b for b, m in corr])
        x = [m for b, m in corr]
        bar_width = x[1] - x[0]

        log_plot = plt.bar(x, y, width = bar_width, align = 'center', color = '#1f77b4', edgecolor = '#1f77b4')

        if var == 'dCtrl':
            # quadratic fit to log data to compare distribution to Gaussian
            qfit = np.polyfit(x, y, 2)
            f = np.poly1d(qfit)
            x_qfit = np.linspace(x[0], x[-1], 100)
            y_qfit = f(x_qfit)
            plt.plot(x_qfit, y_qfit, linestyle = '-', color = 'r')

            # formatting equation of fit
            signs = []
            for q in qfit:
                sign = np.sign(q)
                if sign >= 0:
                    signs.append('+')
                else:
                    signs.append('')

            # display equation
            plt.text(min(x), max(y), '$y = %.2fx^{2} %s %.2fx %s %.2f$' % (qfit[2],signs[1],qfit[1],signs[0],qfit[0]))

        # more formatting
        plt.ylim(0,)
        plt.title('%s | Logarithm Plot\nMean: %.4e, Std: %.4e | Entries: %i' % (filename, mean, stdev, n))
        plt.xlabel(var)
        plt.ylabel('ln(Counts)')
        plt.savefig('plots/histograms/%s/%sLog%s - %i Bins.png' % (filename, filename, var, bin_num))
        plt.clf()

        print('\nLog histogram png saved at: plots/histograms/%s/%sLog%s - %i Bins.png\n' % (filename, filename, var, bin_num))

def main(run_name, date):
    plotHistogram(run_name, date, 'dCtrl', True)
    plotHistogram(run_name, date, 'Ctrl', False)

if __name__ == '__main__':

    # process sys args
    if len(sys.argv) == 3:
        run_name = sys.argv[1]
        date = sys.argv[2]

        # run main
        main(run_name, date)

    else:
        print('''
        %s - Plots histograms over a match type.
        Args:
        [1] - Run Type (HomeTeam:smrtcnt:AwayTeam:smrtcnt)
        [2] - Date (DD.MM)
        ''' % sys.argv[0])
        exit()
