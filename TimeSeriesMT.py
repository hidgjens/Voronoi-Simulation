import pandas as pd
import sys
from os import listdir
from tqdm import tqdm
import subprocess as s
import threading
import time
from scripts.TimeSeriesPlot import plotBatch

def MakeBatches(filename):
    processes = 8
    # split files to load into batches
    datafiles = [file for file in listdir('data_files/csvs/%s' % filename) if file.split('.')[-1] == 'csv']
    num_files = len(datafiles)
    print('%s: %i csv files found' % (filename, num_files))

    batchsize = num_files // processes # files per thread
    remainder = num_files % processes # extras

    batches = []
    # add files to batches    
    for i in range(processes):
        startpoint = batchsize * i
        endpoint = batchsize * (i+1) - 1
        batches.append(datafiles[startpoint:endpoint])        
    
    # split remainders between batches
    for i in range(remainder):
        batch_num = i % 8
        batches[batch_num].append(datafiles[-i])    

    return(batches)

def LoadBatch(batch, filename):
    df = pd.DataFrame()
    for datafile in tqdm(batch):
        # load csv
        dat = pd.read_csv('data_files/csvs/%s/%s' % (filename, datafile), sep = '\t', index_col = 0)
        match_num = datafile.split('.')[1]        
        # assign each match an MID
        dat['MID'] = [match_num] * len(dat.index)
        df = df.append(dat, ignore_index = True)

    print('Finished. %i files loaded.' % len(batch))
    return(df)

# get list of MIDs
def GetBatchMatchList(batch):
    match_nums = []
    for datafile in batch:
        match_nums.append(datafile.split('.')[1]) 
    
    return(match_nums)
    
def main(run_name, date):
    filename = '%s.%s' % (date, run_name)
    batches = MakeBatches(filename)
    
    # create worker function
    def worker(batch):
        df = LoadBatch(batch, filename)
        match_nums = GetBatchMatchList(batch)
        # worker carries out match-level analysis on batch (time series & vorplots)
        plotBatch(df, match_nums, run_name, date)

    # start threads
    threads = [threading.Thread(target=worker, args=(batch, filename)) for batch in batches]
    for thread in threads:
        thread.start()

if __name__ == '__main__':
    # process sys args
    if len(sys.argv) == 3:
        run_name = sys.argv[1]
        date = sys.argv[2]
        main(run_name, date)
    
    else:
        print('''
        %s - Plots fitted histograms over a match type.
        Args:
        [1] - filename (DD.MM.MatchConf:HomeTeam:AwayTeam)
        ''' % sys.argv[0])
        exit()