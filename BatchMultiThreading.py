import pandas as pd
import sys
from os import listdir
from tqdm import tqdm
import subprocess as s
import threading
import time

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

def LoadBatch(batch):
    df = pd.DataFrame()
    for i, datafile in tqdm(enumerate(batch)):
        # load csv
        dat = pd.read_csv('data_files/csvs/%s/%s' % (filename, datafile), sep = '\t', index_col = 0)
                
        # assign each match an MID
        dat['MID'] = [i] * len(dat.index)
        df = df.append(dat, ignore_index = True)

    print('Finished. %i files loaded.' % len(batch))
    return(df)


def main(filename, batchsize):
    batches = MakeBatches(filename, batchsize)
    
    # create worker function
    def worker(batch):
        df = LoadBatch(batch)
        # worker carries out match-level analysis on batch (time series & vorplots)
        cmdlist = ['scripts/TimeSeriesPlot.py', df]
        s.call(cmdlist)

    # start threads
    threads = [threading.Thread(target=worker, args=(batch)) for batch in batches]
    for thread in threads:
        thread.start()

if __name__ == '__main__':
    # process sys args
    if len(sys.argv) == 3:
        filename = sys.argv[1]
        batchsize = sys.argv[2]
        main(filename)
    
    else:
        print('''
        %s - Plots fitted histograms over a match type.
        Args:
        [1] - filename (DD.MM.MatchConf:HomeTeam:AwayTeam)
        ''' % sys.argv[0])
        exit()
    
