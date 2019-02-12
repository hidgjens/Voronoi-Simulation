import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import sys
from os.path import exists
from os import makedirs

# load match
def LoadGame(filename, matchnum):
    df = pd.DataFrame()
    dat = pd.read_csv('data_files/csvs/%s/%s.csv' % (filename, matchnum), sep = '\t', index_col = 0)
    df = df.append(dat, ignore_index = True)

    return(df)


# euclidean distance between 2 points
def EucDist(point1, point2):
    x1, y1 = point1
    x2, y2 = point2
    dist = np.sqrt((x1-x2)^2 + (y1-y2)^2)
    return dist


# calculate distances between array points and player position
def Dist2Plyr(plyrpos, array):
    distarray = np.empty((105,68))

    for i, j in np.ndindex((105,68)):
        distarray[i,j] = eucdist(plyrpos, array[i,j])        
    
    return(distarray)


# generate array of unit squares with their pitch co-ordinates
def GenerateArray():
    # pitch dimensions
    PitchX = 105
    PitchY = 68

    # generate empty array
    array = np.empty((105,68))
    
    # fill with their coordinates
    for i, j in np.ndindex((105,68)):
        # want to go from i, j -> x, y = i, j -PitchX,Y/2 + 1/2 (so i, j = 0, 0 maps to x, y = -33.5, -52) 
        array[i,j] = (i - (PitchX - 1)/2, j - (PitchY -1)/2)

    return(array)


def MinDistPlayers(pitcharray, frame):
    plyrposlist = frame[['X', 'Y']].values
    plyrlist = frame[['Team','Num']].values
    start = False

    for plyrpos, plyr in zip(plyrposlist, plyrlist):
        # calculate distances of all points to player
        distarray = Dist2Plyr(plyrpos, pitcharray) 

        # first player initially has all minimum distances
        if not start:
            minarray = distarray
            ctrlarray = np.empty((105,68))
            start = True
        
        else:
            # if dist < min dist, replace player in ctrlarray and update minarray
            for i,j in np.ndindex((105,68)):
                if distarray[i,j] < minarray[i,j]:
                    minarray[i,j] = distarray[i,j]
                    ctrlarray[i,j] = plyr

    return(ctrlarray)


# plot each frame in list of frames
def PlotFrames(df, frames):
    for fid in range(frames):
        frame = df[(df['FID'] == fid) & (df['Team'] != 'Ball')].reset_index()

        # for a frame, get positions of players, their team and spatial control
        frame = frame[['X','Y','Team','Num','Ctrl']]
        ctrlarray = MinDistPlayers(GenerateArray(), frame)

        #  convert ctrlarray to colours
        for i,j in np.ndindex((105,68)):
            plyr = ctrlarray[i,j]
            team = plyr[0]

            if team == 'Home':
                ctrlarray[i,j] = 1
            elif team == 'Away':
                ctrlarray[i,j] = 0
        
        ax = sns.heatmap(ctrlarray, cmap = 'coolwarm')
        ax.set_aspect("equal")
        
        # better safe than sorry
        if not exists('plots/heatmaps/test'):
            makedirs('plots/heatmaps/test')
        
        plt.savefig('plots/heatmaps/test/test%i.png' % fid)

def main(filename, matchnum, frames):
    df = LoadGame(filename, matchnum)
    PlotFrames(df, frames)
    
if __name__ == '__main__':
    # process sys args
    if len(sys.argv) == 4:
        filename = sys.argv[1]
        matchnum = sys.argv[2]
        frames = int(sys.argv[3])
    else:
        print('''
        %s - Plots heatmap of spatial control to show sampling.
        Args:
        [1] - Filename
        [2] - Match number
        [3] - Number of frames
        ''' % sys.argv[0])
        exit()

    main(filename, matchnum, frames)