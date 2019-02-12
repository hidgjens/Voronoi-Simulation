import VorPlot as vplot
import subprocess as s
import sys
from os.path import exists
from os import makedirs

def CreateVid(run_name, number):
    name = run_name + '_' + number
    # create video subdirectory
    dirname = run_name
    if not exists('videos/vorplots_%s' % dirname):
        makedirs('videos/vorplots_%s' % dirname)

    # create pngs
    vplot.iterPlot(run_name, number)

    # create vid from pngs
    s.call(['ffmpeg','-framerate','5','-i','plots/vorplots/vorplots_%s/voronoi-%%04d.png' % name,'videos/vorplots_%s/vorplots_%s.mp4' % (dirname, name),'-vcodec','mpeg4','-y'])

def main(run_name, vid_num):
    #if not vid_num == 0: < not necessary, range(0) just wont run at all
    for number in range(vid_num):
        #filename = '%s_%i' % (name, j)
        CreateVid(run_name, str(number))

if __name__ == '__main__':
    # sys args
    if len(sys.argv) == 4:
        run_name = sys.argv[1]
        date = sys.argv[2]
        vid_num = int(sys.argv[3])
        
    else:
        print('''
        %s - Plots Voronoi diagrams over matches and plots vids
        Args:
        [1] - Run name (MatchConf:HomeTeam:AwayTeam)
        [2] - Date (DD.MM)
        [3] - Number of matches to plot
        ''' % sys.argv[0])
        exit()

    # now run main
    filename ='%s.%s' % (date, run_name)
    main(filename, vid_num)