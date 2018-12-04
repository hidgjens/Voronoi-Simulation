import pandas as pd
import numpy as np
import matplotlib as mpl
mpl.use('agg')
import matplotlib.pyplot as plt
from os import makedirs, listdir
from os.path import exists
from HistPlot import filter, plotHistogram
import sys

def LoadRunName(run_name, date):
    filename = '%s.%s' % (date, run_name)
    datafiles = [file for file in listdir('data_files/csvs/%s' % run_name) if file.split('.')[-1] == 'csv']
    num_files = len(datafiles)
    print('%s: %i csv files found' % (filename, num_files))

    ctrl = []
    dctrl = []

    for i, datafile in enumerate(datafiles):
        # print every 10%
        if num_files > 10 and i % (num_files // 10) == 0:
            print('%i%%: loading %s' % (int(100 * i / num_files), datafile))
        
        # load csv
        df = pd.read_csv('data_files/csvs/%s/%s' % (run_name, datafile), sep = '\t', index_col = 0)
        
        # apply filters
        df = filter(df, run_name)
        
        # calculate total team Ctrl or dCtrl in each frame and add to list
        # we implicitly remove frame 0 here
        Frames = df['FID'].max()
        for j in range(Frames):
            frame = df[df['FID'] == j + 1]
            ctrl.append(frame['Ctrl'].sum())
            dctrl.append(frame['dCtrl'].sum())

    teamdf = pd.DataFrame(list(zip(ctrl, dctrl)), columns = ['Ctrl', 'dCtrl'])
    print(teamdf)
    print('100%: finished.')
    return(teamdf)

def plotTeamHistogram(data, run_name, date, var, log, team, title):
    filename = '%s.%s' % (date, run_name)

    if title == None:
        plotHistogram(data, run_name, date, var, log, title = '%s.%s' % (filename, team))    
    else:
        plotHistogram(data, run_name, date, var, log, title = title)

def main(run_name, date):
    # load data
    df = LoadRunName(run_name, date)

    # plot histograms
    plotTeamHistogram(df, run_name, date, 'Ctrl', log = False, team = 'Home', title = None)
    plotTeamHistogram(df, run_name, date, 'dCtrl', log = True, team = 'Home', title = None)

if __name__ == '__main__':
    # process sys args
    if len(sys.argv) == 3:
        run_name = sys.argv[1]
        date = sys.argv[2]

        # run main
        main(run_name, date)

    else:
        print('''
        %s - Plots team-level histograms over a match type.
        Args:
        [1] - Run Type (HomeTeam:smrtcnt:AwayTeam:smrtcnt)
        [2] - Date (DD.MM)
        ''' % sys.argv[0])
        exit()