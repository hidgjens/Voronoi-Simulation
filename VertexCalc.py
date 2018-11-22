import pandas as pd
import numpy as np
import sys
from os import makedirs, listdir
from os.path import exists
from tqdm import tqdm
from scipy.spatial import Voronoi

def MakeBatches(filename, batchsize):
    # split files to load into batches of 100 matches
    datafiles = [file for file in listdir('data_files/csvs/%s' % filename) if file.split('.')[-1] == 'csv']
    num_files = len(datafiles)
    print('%s: %i csv files found' % (filename, num_files))

    num_batches = (num_files // 100) + 1
    batches = []
    
    for i in range(num_batches):
        startpoint = batchsize * i
        
        if i == num_batches - 1:
            endpoint = num_files - 1
        else:
            endpoint = (batchsize * (i+1)) - 1 

        batches.append(datafiles[startpoint:endpoint])        
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

def getVertices(df):
    # filter out balls
    df = df.loc[df['Team'] != 'Ball']
    matches = 1 + df['MID'].max()
    frames = 1 + df['FID'].max()
    entries = frames * matches
    vtxpp = 0

    for i in tqdm(range(matches)):
        # get match
        mdf = df[df['MID'] == i]    
        for j in range(frames):
            # get frame
            frame = mdf[mdf['FID'] == j]
            players = frame.shape[0]

            # compute voronoi diagram and get number of vertices
            positions = frame[['X', 'Y']].values
            vertices = len(Voronoi(positions).ridge_vertices)
            
            # update vertices per player
            # divide by players/2 becaaause by definition 2 players share an edge
            vtxpp += 2 * vertices / players
    
    # return number of vertices and entries
    return(vtxpp, entries)

def sumBatches(batches):
    # entries, vertices per player, vertices per player per frame
    entries = vtxpp = vtxpppf = 0
    for batch in batches:
        v, e = getVertices(LoadBatch(batch))
        entries += e
        vtxpp += v
        vtxpppf = vtxpp / entries
        print('Current estimate: %f' % vtxpppf)
    
    return(vtxpppf, entries)

def main(filename, batchsize):
    batches = MakeBatches(filename, batchsize)
    vtxpppf, entries = sumBatches(batches)

    # print and save result to file
    print('Mean Voronoi edges per player per frame: %f' % vtxpppf)
    if not exists('plots/vertex_count/%s' % filename):
        makedirs('plots/vertex_count/%s' % filename)

    f = open('plots/vertex_count/%s' % filename, 'w')
    f.write('Mean Voronoi edges per player per frame: %f\n' % vtxpppf)
    f.write('Entries: %i' % entries)
    f.close()

    print('Saved to file: plots/vertex_count/%s.txt' % filename)
    print('Finished.')

if __name__ == '__main__':
    # process sys args
    if len(sys.argv) == 3:
        filename = sys.argv[1]
        batchsize = int(sys.argv[2])

        # run main
        main(filename, batchsize)

    else:
        print('''
        %s - Calculates average vertices per player per frame.
        Args:
        [1] - filename (name of folder to pull csvs from)
        [2] - batchsize 
        ''' % sys.argv[0])
        exit()    