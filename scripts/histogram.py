import matplotlib as mpl
mpl.use('agg')
import matplotlib.pyplot as plt
import pandas as pd
from os import makedirs, listdir
from os.path import exists
import sys

def plotHistogram(values, bins, filename, name_num = 0):
    if not exists('plots'):
        makedirs('plots')
    if not exists('plots/histograms'):
        makedirs('plots/histograms')
    entries = len(values)
    bins, ranges, patches = plt.hist(values, bins)

    Px = 0
    Px2 = 0

    for quant, lower, upper in zip(bins, ranges[:-1], ranges[1:]):
        middle_value = (upper + lower) / 2
        Px += quant * middle_value
        Px2 += quant * (middle_value ** 2)
    mean = Px / entries
    std = Px2 / entries - (mean) ** 2


    plt.title('Mean: %.4f, Std: %.4f | Entries: %i' % (mean, std, entries))
    plt.savefig('plots/histograms/%s%i.png' % (filename, name_num))





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

    plotHistogram(data, 30, file_name)
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
