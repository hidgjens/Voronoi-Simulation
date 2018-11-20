import VorPlot as vplot
import subprocess as s
import sys
from os.path import exists
from os import makedirs

def CreateVid(name):
    # create video subdirectory
    dirname = name.split('_')[0]
    if not exists('videos/vorplots_%s' % dirname):
        makedirs('videos/vorplots_%s' % dirname)

    # create pngs
    vplot.iterPlot(name)

    # create vid from pngs
    s.call(['ffmpeg','-framerate','5','-i','plots/vorplots/vorplots_%s/voronoi-%%04d.png' % name,'videos/vorplots_%s/vorplots_%s.mp4' % (dirname, name),'-vcodec','mpeg4','-y'])

def main(name, vid_num):
    if not vid_num == 0:
        for j in range(vid_num):
            filename = '%s_%i' % (name, j)
            CreateVid(filename)

if __name__ == '__main__':
    name = sys.argv[1]
    vid_num = int(sys.argv[2])
    main(name, vid_num)
