import scripts.VoronoiPlotFromFile as vplot
import subprocess as s
import sys
from os.path import exists
from os import makedirs



def CreateVid(name, **kwargs):
    # defaults to all frames starting at first frame
    FrameNum = 0
    Frames = 0

    if 'frame1' in kwargs.keys():
        FrameNum = kwargs['frame1']
    if 'frames' in kwargs.keys():
        Frames = kwargs['frames']

    # create video subdirectory
    if not exists('videos'):
        makedirs('videos')

    # gives pngs
    vplot.PlotVoronois(name, **kwargs)

    # create vid from pngs
    s.call(['ffmpeg','-framerate','5','-i','plots/voronoi_plots_%s/voronoi-%%04d.png' % name,'videos/vor_plot_%s.mp4' % name,'-vcodec','mpeg4','-y'])

# allows us to run directly in the terminal using system arguments
def main(name, init_frame, frame_num):
        CreateVid(name, frame1 = init_frame, frames = frame_num)

if __name__ == '__main__':
    # system arguments are name (name of csv to plot from), frame1 (first frame to plot), frames (total number of frames to plot - if frames = 0 all frames are plotted)
    name = sys.argv[1]
    frame1 = int(sys.argv[2])
    frames = int(sys.argv[3])
    main(name, frame1, frames)
