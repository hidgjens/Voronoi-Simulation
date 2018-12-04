import pandas as pd
import sys
import multiprocessing
import subprocess as s
from os.path import exists
from os import makedirs
from os import listdir
from tqdm import tqdm
from BatchVorPlot import PlotFrames


# split match into batches
def LoadMatch(filename, filenum):
    df = pd.read_csv('data_files/csvs/%s/%s.csv' % (filename, filenum), sep = '\t', index_col = 0)
    return(df)

# process each batch
def MakeBatches(df):
    processes = 8
    # split files to load into batches
    frames = 1 + df['FID'].max()
    print('Frames: %s' % frames)
    framelist = range(1,frames)

    batchsize = frames // processes # files per thread
    remainder = frames % processes # extras

    batches = []
    # add files to batches    
    for i in range(processes):
        startpoint = batchsize * i
        endpoint = batchsize * (i+1)
        batches.append(framelist[startpoint:endpoint])        
    
    # split remainders between batches
    for i in range(remainder):
        frame = frames - (i+1)
        batches[i].append(frame)

    return(batches)

# plot each batch in a subprocess
def PlotMatchMT(filename, filenum):
    match = LoadMatch(filename, filenum)
    batches =  MakeBatches(match)

    # create worker function
    def worker(batch, filename):
        PlotFrames(match, batch, filename)

    # start processes

    procs = [multiprocessing.Process(target=worker, args=(batch, filename)) for batch in batches]
    for p in procs:
        p.start()

    # wait until processes are finished before creating vid
    finished = False
    while not finished:
        if all(p.is_alive() == False for p in procs):    
            finished = True
            return(finished)

# create vorvids once finished
def CreateVid(filename, filenum):
    # create video subdirectory
    if not exists('videos/vorplots_%s' % filename):
        makedirs('videos/vorplots_%s' % filename)

    # create vid from pngs
    s.call(['ffmpeg','-framerate','5','-i','plots/vorplots/vorplots_%s/voronoi-%%04d.png' % filename,'videos/vorplots_%s/%i.mp4' % (filename,filenum),'-vcodec','mpeg4','-y'])

def main(run_name, date, vid_num):
    filename = '%s.%s' % (date, run_name)
    for i in range(vid_num):
        # plot pngs
        finished = PlotMatchMT(filename, i)
        # make vid when pngs are all plotted
        if finished:
            CreateVid(filename, i)

if __name__ == '__main__':
    # process sys args
    if len(sys.argv) == 4:
        run_name = sys.argv[1]
        date = sys.argv[2]
        vid_num = int(sys.argv[3])
        main(run_name, date, vid_num)
    
    else:
        print('''
        %s - Plots Voronoi diagrams (multithreaded) over matches and plots vids
        Args:
        [1] - Run Name (MatchConf:HomeTeam:AwayTeam)
        [2] - Date (DD.MM)
        [3] - Number of matches to plot
        ''' % sys.argv[0])
        exit()