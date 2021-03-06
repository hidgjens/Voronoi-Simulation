import numpy as np
import matplotlib as mpl
mpl.use('agg')
import pandas as pd
import matplotlib.cm as cm
import matplotlib.pyplot as plt
import sys
from tqdm import tqdm
from os.path import exists
from os import makedirs
from scipy.spatial import Voronoi, voronoi_plot_2d

# placeholder values
PitchX = 105
PitchY = 68

def importFrame(run_name, number):
    
    # pull data from csv into dataframe
    df = pd.read_csv('data_files/csvs/%s/%s.csv' % (run_name, number), sep = '\t', index_col = 0)
    # delete excess columns
    # df = df.drop(columns = ['PID','Time'])

    return(df)

# from stackoverflow: https://stackoverflow.com/questions/20515554/colorize-voronoi-diagram
# allows us to colourise Voronoi plot
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

# compute Voronoi tessellation for all frames in df
def iterPlot(run_name, number):
    # import dataframe
    df = importFrame(run_name, number)

    Frames = 1 + df['FID'].max()
    print('Frames: ', Frames)

    # plot Voronoi diagram for each frame
    for i in tqdm(range(Frames)):
        # create frame dataframe
        frame = df.loc[df['FID'] == i, ['FID', 'X', 'Y', 'Team', 'Ctrl', 'Smart', 'Possession']]
        name = run_name + '_' + number
        PlotVoronois(name, frame, i)

def PlotVoronois(name, frame, framenum):
    frame = frame[frame['Team'] != 'Ball'].reset_index()
    # get array of player positions and possession value
    pos_array = frame[['X', 'Y', 'Ctrl']].values
    poss = frame.at[1, 'Possession']

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
            # colour smart players differently so we can identify them more easily
        #    if frame.at[j, 'Smart']:
        #        TeamColour = 'm'

        elif frame.at[j, 'Team'] == 'Away':
            TeamColour = 'b'
        #    if frame.at[j, 'Smart']:
        #        TeamColour = 'c'
            
        plt.plot(pos_array[j,0], pos_array[j,1], 'o', color = TeamColour)

        # write spatial control (in %) of each player near them
        plt.text(pos_array[j,0] + 0.8, pos_array[j,1] + 0.8,  '%.1f' % (100 * pos_array[j,2]), fontsize = 8)

        # write current possession state on plot
        if poss:
            posstext = 'Possession: Home'
            posscolour = 'r'
        else:
            posstext = 'Possession: Away'
            posscolour = 'b'
        plt.text((-PitchX/2)+1, (PitchY/2)+1, posstext, fontsize = 12, color = posscolour)

    plt.gca().set_aspect('equal')
    plt.xlim(-0.5 * PitchX, 0.5 * PitchX)
    plt.ylim(-0.5 * PitchY, 0.5 * PitchY)

    # check if output folder exists
    if not exists('plots/vorplots/vorplots_%s' % name):
        makedirs('plots/vorplots/vorplots_%s' % name)

    # save plot
    plt.savefig('plots/vorplots/vorplots_%s/voronoi-%04d.png' % (name, framenum))
    plt.clf()

def main(run_name, number):
    iterPlot(run_name, number)

if __name__ == '__main__':
    run_name = sys.argv[1]
    number = sys.argv[2]
    main(run_name, number)