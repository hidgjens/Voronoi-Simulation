import pandas as pd
import numpy as np
from os import listdir
from os.path import exists
from scripts.VorPlot import PlotVoronois
import sys

def importFrame(name):
    df = pd.read_csv('data_files/csvs/%s.csv' % name, sep = '\t', index_col = 0)
    
    # delete excess columns
    df = df.drop(columns = ['PID','Time'])
    df = df.loc[df['Team'] != 'Ball']
    
    return(df)

def plotZeroCtrlFrames(name, dataframe):
    # find frames in which a player has zero Ctrl
    df = dataframe.loc[dataframe['Ctrl'] == 0]
    # return list of FIDs corresponding to these results
    fids = df['FID'].values

    for fid in fids:
        frame = dataframe.loc[dataframe['FID'] == fid]
        PlotVoronois(name, frame, fid)

def main(run_name, date):
    # search directory for files
    filename = '%s.%s' % (date, run_name)
    datafiles = [file for file in listdir('data_files/csvs') if file.split('_')[0] == filename]
    num_files = len(datafiles)
    print('%i files found.' % num_files)

    # plot 0 Ctrl frames for each file 
    for i in range(num_files):

        name = '%s_%s' % (filename, i)
        dataframe =  importFrame(name)
        plotZeroCtrlFrames(name, dataframe)
        print('Plotting finished for file %i.' % i)

if __name__ == '__main__':
    # process sys args
    if len(sys.argv) == 3:
        run_name = sys.argv[1]
        date = sys.argv[2]
        # run main
        main(run_name, date)

    else:
        print('''
        %s - Plots frames containing players with 0 Ctrl from a set of matches.
        Args:
        [1] - Run Name (HomeTeam:smrtcnt:AwayTeam:smrtcnt)
        [2] - Date (DD.MM)
        ''' % sys.argv[0])
        exit()