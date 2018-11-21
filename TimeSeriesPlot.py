import pandas as pd
import numpy as np
import matplotlib as mpl
mpl.use('agg')
import sys
import matplotlib.cm as cm
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from os import makedirs, listdir
from os.path import exists
from PeakFitting import filter

def LoadByGame(filename):
    datafiles = [file for file in listdir('data_files/csvs/%s' % filename) if file.split('.')[-1] == 'csv']
    num_files = len(datafiles)
    print('%s: %i csv files found' % (filename, num_files))

    df = pd.DataFrame()
    for i, datafile in enumerate(datafiles):
        # print every 10%
        if num_files > 10 and i % (num_files // 10) == 0:
            print('%i%%: loading %s' % (int(100 * i / num_files), datafile))
        # load csv
        dat = pd.read_csv('data_files/csvs/%s/%s' % (filename, datafile), sep = '\t', index_col = 0)
                
        # assign each match its unique ID from csv name
        # split filename by '_' (returns 'X.csv'), then split by '.' to get 'X'
        match_num = int(datafile.split('.')[0])
        dat['MID'] = [match_num] * len(dat.index)
        # append match to dataframe
        df = df.append(dat, ignore_index = True)

    print('100%: finished.')
    return(df)

# plots time series graphs for a given match
def TimeSeriesPlot(df, var, title, filename):    
    # define useful quantities
    frames =  1 + df['FID'].max()
    players = df['Num'].max()
    t = range(1, frames) 
    colours = cm.plasma(np.linspace(0, 1, players)) 
    patches = []

    print('Frames: %i' % frames)
    plt.figure(figsize = (21, 7), dpi = 300) 

    for i in range(players):
        # get player data
        plyrdf = df.loc[df['Num'] == i + 1]
        plyrdata = plyrdf[var].tolist()

        # plot
        plt.plot(t, plyrdata, color = colours[i], linestyle = '-', linewidth = 0.75)
        # print('Player plotted: %i' % (i + 1))

        patches.append(mpatches.Patch(color = colours[i], label = 'Player %i' % (i + 1)))

    # formatting
    plt.xlim(0,)
    plt.ylim(0,)
    plt.xlabel('Frame')
    plt.ylabel(var)
    plt.title(title)
    
    # legend
    plt.legend(handles = patches, loc = 1)

    # better safe than sorry
    if not exists('plots/time_series/%s' % filename):
        makedirs('plots/time_series/%s' % filename)

    plt.savefig('plots/time_series/%s/%s.png' % (filename, title))
    print('Time series saved at: plots/time_series/%s/%s.png' % (filename, title))
    plt.close()

def TimeTeam(df, var, title, filename):    
    # define useful quantities
    tmdata = df[var].tolist()
    frames =  1 + df['FID'].max()
    t = range(1, frames)

    print('Frames: %i' % frames)
    plt.figure(figsize = (21, 7), dpi = 300) 
    
    # plot
    plt.plot(t, tmdata, linestyle = '-', linewidth = 0.75)

    # formatting
    plt.xlim(0,)
    plt.ylim(0,)
    plt.xlabel('Frame')
    plt.ylabel(var)
    plt.title(title)

    # better safe than sorry
    if not exists('plots/time_series/%s' % filename):
        makedirs('plots/time_series/%s' % filename)

    plt.savefig('plots/time_series/%s/%s.png' % (filename, title))
    print('Time series saved at: plots/time_series/%s/%s.png' % (filename, title))
    plt.close()

# plots time series graphs for all matches in a given run
def PlotMatches(df, var, title, filename):
    matches = 1 + df['MID'].max()

    # split dataframe into matches
    for i in range(matches):
        mdf = df.loc[df['MID'] == i]
        # pass match to plot
        TimeSeriesPlot(mdf, var, '%s | Match %i' % (title, i), filename)
        
        # filter out single player for team plot
        mdf = mdf.loc[(mdf['Team'] == 'Home') & (mdf['Num'] == 1)]
        # plot with team variable
        TimeTeam(mdf, 'Tm%s' % var, '%s | Team Plot | Match %i' % (title, i), filename)
        print('Match %i plotted.' % i)

def main(run_name, date):
    filename = '%s.%s' % (date, run_name)    
    # pre-filter dataframe
    df = filter(LoadByGame(filename), run_name)
    # plot graphs
    PlotMatches(df, 'Ctrl', '%s | Time Series' % filename, filename)

if __name__ == '__main__':
    # process sys args
    if len(sys.argv) == 3:
        run_name = sys.argv[1]
        date = sys.argv[2]

        # run main
        main(run_name, date)

    else:
        print('''
        %s - Plots time series over a match type.
        Args:
        [1] - Run Type (HomeTeam:smrtcnt:AwayTeam:smrtcnt)
        [2] - Date (DD.MM)
        ''' % sys.argv[0])
        exit()
