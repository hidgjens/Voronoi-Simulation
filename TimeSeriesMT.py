import pandas as pd
import sys
from os import listdir
from tqdm import tqdm
import subprocess as s
import multiprocessing
from scripts.BatchTimeSeries import plotBatch

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
        endpoint = batchsize * (i+1)
        batches.append(datafiles[startpoint:endpoint])        
    
    # split remainders between batches
    for i in range(remainder):
        match_num = num_files - (i+1)
        batches[i].append(datafiles[match_num])

    return(batches)

def LoadBatch(batch, filename):
    df = pd.DataFrame()
    for datafile in tqdm(batch):
        # load csv
        dat = pd.read_csv('data_files/csvs/%s/%s' % (filename, datafile), sep = '\t', index_col = 0)
        match_num = int(datafile.split('.')[0])        
        # assign each match an MID
        dat['MID'] = [match_num] * len(dat.index)
        df = df.append(dat, ignore_index = True)

    print('Finished. %i files loaded.' % len(batch))
    return(df)
    
def main(run_name, date):
    filename = '%s.%s' % (date, run_name)
    batches = MakeBatches(filename)
    print(batches)
    # create worker function
    def worker(batch, filename):
        df = LoadBatch(batch, filename)
        match_nums = []
        
        for datafile in batch:
            match_nums.append(int(datafile.split('.')[0]))
        # plot time series for matches in batch
        plotBatch(df, match_nums, run_name, date)

    # start processes
    procs = [multiprocessing.Process(target=worker, args=(batch, filename)) for batch in batches]
    for p in procs:
        p.start()

if __name__ == '__main__':
    # process sys args
    if len(sys.argv) == 3:
        run_name = sys.argv[1]
        date = sys.argv[2]
        main(run_name, date)
    
    else:
        print('''
        %s - Plots time series (multithreaded) over a match type.
        Args:
        [1] - Run Name (MatchConf:HomeTeam:AwayTeam)
        [2] - Date (DD.MM)
        ''' % sys.argv[0])
        exit()