import pandas as pd
import numpy as np
import matplotlib as mpl
mpl.use('agg')
import matplotlib.cm as cm
import matplotlib.pyplot as plt
import sys
import time
from multiprocessing import Process
from matplotlib import animation
from os.path import exists
from os import makedirs, listdir
from scipy.spatial import Voronoi, voronoi_plot_2d


# placeholder values
PitchX = 105
PitchY = 68


def voronoi_finite_polygons_2d(vor, radius=1000):
    """
    Reconstruct infinite voronoi regions in a 2D diagram to finite
    regions.

    Parameters
    ----------
    vor : Voronoi
        Input diagram
    radius : float, optional
        Distance to 'points at infinity'.

    Returns
    -------
    regions : list of tuples
        Indices of vertices in each revised Voronoi regions.
    vertices : list of tuples
        Coordinates for revised Voronoi vertices. Same as coordinates
        of input vertices, with 'points at infinity' appended to the
        end.
    """

    if vor.points.shape[1] != 2:
        raise ValueError("Requires 2D input")

    new_regions = []
    new_vertices = vor.vertices.tolist()

    center = vor.points.mean(axis=0)
    if radius is None:
        radius = vor.points.ptp().max()

    # Construct a map containing all ridges for a given point
    all_ridges = {}
    for (p1, p2), (v1, v2) in zip(vor.ridge_points, vor.ridge_vertices):
        all_ridges.setdefault(p1, []).append((p2, v1, v2))
        all_ridges.setdefault(p2, []).append((p1, v1, v2))

    # Reconstruct infinite regions
    for p1, region in enumerate(vor.point_region):
        vertices = vor.regions[region]

        if all(v >= 0 for v in vertices):
            # finite region
            new_regions.append(vertices)
            continue

        # reconstruct a non-finite region
        ridges = all_ridges[p1]

        new_region = [v for v in vertices if v >= 0]

        for p2, v1, v2 in ridges:
            if v2 < 0:
                v1, v2 = v2, v1
            if v1 >= 0:
                # finite ridge: already in the region
                continue

            # Compute the missing endpoint of an infinite ridge

            t = vor.points[p2] - vor.points[p1] # tangent
            t /= np.linalg.norm(t)
            n = np.array([-t[1], t[0]])  # normal

            midpoint = vor.points[[p1, p2]].mean(axis=0)
            direction = np.sign(np.dot(midpoint - center, n)) * n
            far_point = vor.vertices[v2] + direction * radius

            new_region.append(len(new_vertices))
            new_vertices.append(far_point.tolist())

        # sort region counterclockwise
        vs = np.asarray([new_vertices[v] for v in new_region])
        c = vs.mean(axis=0)
        angles = np.arctan2(vs[:,1] - c[1], vs[:,0] - c[0])
        new_region = np.array(new_region)[np.argsort(angles)]

        # finish
        new_regions.append(new_region.tolist())
    return new_regions, np.asarray(new_vertices)



def UpdateFrame(df, matchnum, framenum, filename):
    frame = df[(df['FID'] == framenum) & (df['Team'] != 'Ball')].reset_index()
    ball_df = df[(df['FID'] == framenum) & (df['Team'] == 'Ball')].reset_index()
    ball_pos = [ball_df['X'].values[0], ball_df['Y'].values[0]]
    # get array of player positions and possession value
    pos_array = frame[['X', 'Y', 'Ctrl']].values
    poss = frame.at[1, 'Possession']
    
    # get team ctrl values
    home_ctrl = 100 * float(frame.at[1, 'TmCtrl'])
    away_ctrl = 100 - home_ctrl

    # get number of players in frame
    HomePlayers = len(frame.loc[frame['Team'] == 'Home'].index)
    AwayPlayers = len(frame.loc[frame['Team'] == 'Away'].index)
    AllPlayers = HomePlayers + AwayPlayers

    # compute Voronoi tessellation from player positions
    vor = Voronoi(pos_array[:,:2])
    regions, vertices = voronoi_finite_polygons_2d(vor)

    # colourise
    teamkey = np.append(np.ones(HomePlayers)*0.9, np.ones(AwayPlayers)*0.1)
    norm = mpl.colors.Normalize(vmin=0.0, vmax=1.0, clip=True)
    mapper = cm.ScalarMappable(norm=norm, cmap=cm.coolwarm)

    for idx, region in enumerate(regions):
        polygon = vertices[region]
        plt.fill(*zip(*polygon), alpha=0.4, color = mapper.to_rgba(teamkey[idx]))

    # plot
    for j in range(AllPlayers):
        if  frame.at[j, 'Team'] == 'Home':
            TeamColour = 'r'
        elif frame.at[j, 'Team'] == 'Away':
            TeamColour = 'b'
            
        plt.plot(pos_array[j,0], pos_array[j,1], 'o', color = TeamColour)

        # write spatial control (in %) of each player near them
        plt.text(pos_array[j,0] + 0.8, pos_array[j,1] + 0.8,  '%.1f' % (100 * pos_array[j,2]), fontsize = 8)

        # write current possession state and ctrl info on plot
        if poss:
            poss_state = 'Home'
        else:
            poss_state  = 'Away'

        posstext = 'Possession: %s\nHome: %.1f%% Away: %.1f%%' % (poss_state, home_ctrl, away_ctrl)
        info = '%s\nMatch: %i Frame: %i' % (filename, matchnum, framenum)
        plt.text((-PitchX/2)+1, (PitchY/2)+3, posstext, fontsize=11)
        plt.text(0, (PitchY/2)+3, info, fontsize=11)

    plt.plot(ball_pos[0], ball_pos[1], 'o', color='k')
    


# load data from csv
def LoadMatch(filename, matchnum):
    df = pd.read_csv('data_files/csvs/%s/%s.csv' % (filename, matchnum), sep = '\t', index_col = 0)
    return(df)


def CreateAnimation(filename, matchnum):
    # get start time and define useful quantities
    start_time = time.time()
    
    fig, ax = plt.subplots()
    df = LoadMatch(filename, matchnum)
    frames = df['FID'].max() + 1 

    def UpdateFigure(i):
        # give some progress updates
        if frames > 10 and i % (frames // 10) == 0:
            print('Frame: %i (of %i)' % (i, frames))

        # compute frame
        fig.clear()
        UpdateFrame(df, matchnum, i, filename)
        
        # formatting
        plt.gca().set_aspect('equal')
        plt.xlim(-0.5 * PitchX, 0.5 * PitchX)
        plt.ylim(-0.5 * PitchY, 0.5 * PitchY)
        plt.xlabel('x, m')
        plt.ylabel('y, m')
   
    # output file path and name    
    path = "videos/vorplots/%s" % filename
    name = "%i" % matchnum

    if not exists(path):
        makedirs(path)
    
    anim = animation.FuncAnimation(fig, UpdateFigure, frames)
 
    # medium quality is fine
    anim.save("%s/%s.mp4" % (path, name), dpi=200, writer='ffmpeg', bitrate=4096, codec='mpeg4', fps=5)   
    print('Video file saved to: %s/%s.mp4' % (path, name))

    # output performance info
    elapsed_time = time.time() - start_time
    print('Time taken: %.1f s' % elapsed_time)


def main(filename, num_matches):
    # create vid
    for i in range(num_matches):
        p = Process(target=CreateAnimation, args=(filename, i))
        p.start()

if __name__ == '__main__':
    # sys args
    if len(sys.argv) == 3:
        filename = sys.argv[1]
        num_matches = int(sys.argv[2])

    else:
        print('''
        %s - Generates Voronoi animation from file. Args:
        [1] - Filename (name of folder in /csvs/)
        [2] - Number of matches to plot (starts at 0.csv)
        ''' % sys.argv[0])
        exit()
    
    # args sorted now run main
    main(filename, num_matches)