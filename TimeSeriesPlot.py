import pandas as pd
import numpy as np
import matplotlib as mpl
mpl.use('agg')
import sys
import matplotlib.cm as cm
import matplotlib.pyplot as plt
from os import makedirs, listdir
from os.path import exists
from PeakFitting import filter

def LoadByGame(filename):
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
        
        
        # assign each match its unique ID from csv name
        # hacky - replace '.' with '_' then split and take 4th element
        match_num = int(datafile.replace('.','_').split('_')[3])
        dat['MID'] = [match_num] * len(dat.index)
        # append match to dataframe
        df = df.append(dat, ignore_index = True)

    print('100%: finished.')
    return(df)

# plots time series graphs for a given match
def TimeSeriesPlot(df, var, title, filename):    
    frames =  1 + df['FID'].max()
    print('frames: %i' % frames)

    allpoints = []
    for t in range(1, frames):
        # for each frame want to plot each player's Ctrl
        # select frame
        frame = df.loc[df['FID'] == t].reset_index()
        players = frame.shape[0]
        
        # create array of points
        colours = cm.rainbow(np.linspace(0, 1, players))
        values = []
        time = [t] * players
        
        for j in range(players):
            values.append(frame.at[j, var])

        points = zip(time, values, colours)
        allpoints.extend(points)
    
        if frames > 10 and t % (frames // 10) == 0:
            print('Frames computed: %i%%' % (int(100 * t / frames)))

    # plot all points
    x, y, c = zip(*allpoints)
    plt.scatter(x, y, color = c, marker = '+')
    print('Frames plotted')

    # formatting
    plt.ylim(0,)
    plt.xlabel('Frame')
    plt.ylabel(var)
    plt.title(title)
    
    # better safe than sorry
    if not exists('plots/time_series/%s' % filename):
        makedirs('plots/time_series/%s' % filename)

    plt.savefig('plots/time_series/%s/%s.png' % (filename, title))
    print('Time series saved at: plots/time_series/%s/%s.png' % (filename, title))
    plt.clf()

# plots time series graphs for all matches in a given run
def PlotMatches(df, var, title, filename):
    matches = 1 + df['MID'].max()

    # split dataframe into matches
    for i in range(matches):
        mdf = df.loc[df['MID'] == i]
        # pass match to plot
        TimeSeriesPlot(mdf, var, '%s | Match %i' % (title, i), filename)
        print('Match %i plotted:' % i)

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
