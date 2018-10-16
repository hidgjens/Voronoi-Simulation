import numpy as np
import matplotlib as mpl
mpl.use('agg')
import pandas as pd
import matplotlib.cm as cm
import matplotlib.pyplot as plt
from os.path import exists
from os import makedirs
from scipy.spatial import Voronoi, voronoi_plot_2d

# placeholder values:
PitchX = 105
PitchY = 68
def PullFrames(name, FrameNum, Frames, Balls):
    # pull data from csv into dataframe
    df = pd.read_csv('data_files/csvs/%s.csv' % name, sep = '\t', index_col = 0)

    # drop frames outside limits
    if not Frames == 0:
    	df = df[df['FID'] < FrameNum + Frames]
    df = df[df['FID'] >= FrameNum]

    # ignore balls
    if Balls == False:
        df = df[df['Team'] != 'Ball']

    # for plotting Voronois we only need the (FID, Team, Num, X, Y, Ctrl)
    # so we can delete the other columns
    df = df.drop(columns = ['PID','Time'])

#    print(df)
    return(df)

# from stackoverflow: https://stackoverflow.com/questions/20515554/colorize-voronoi-diagram
# makes Voronoi plot able to be coloured
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


# compute Voronoi tessellation for frames in csv
# kwargs are frame1 (initial frame), frames (no. of frames), balls = True/False (include balls or not)
def PlotVoronois(name, **kwargs):
    # defaults to text and no balls
    FrameNum = 0
    Frames = 1
    Balls = False
    Text = True
    # can specify a range of frames starting at frame1, and whether to include balls
    if 'frame1' in kwargs.keys():
        FrameNum = kwargs['frame1']
    if 'balls' in kwargs.keys():
        Balls = kwargs['balls']
    if 'frames' in kwargs.keys():
        Frames = kwargs['frames']
    if 'text' in kwargs.keys():
        Text = kwargs['text']

    # import dataframe
    df = PullFrames(name, FrameNum, Frames, Balls)
    if Frames == 0:
        Frames = len(df.loc[(df['Team'] == 'Home') & (df['Num'] == 1)].values.tolist())
        print('Frames ', Frames)
    # plot Voronoi diagram for each frame
    for i in range(Frames):

        # create list of elements (X, Y, Team, Ctrl) corresponding to each player
        team_frame = df.loc[df['FID'] == FrameNum + i, ['FID', 'X', 'Y', 'Team', 'Ctrl']]
        # reset index so we can iterate through team_frame easily
        team_frame = team_frame.reset_index()
        pos_frame = team_frame.loc[team_frame['Team'] != 'Ball']
        pos_array = pos_frame[['X', 'Y', 'Ctrl']].values
        ball_pos = team_frame.loc[team_frame['Team'] == 'Ball'].values

        # get numbers of home and away players in frame
        HomePlayers = len(team_frame.loc[team_frame['Team'] == 'Home'].index)
#        print(HomePlayers)

        AwayPlayers = len(team_frame.loc[team_frame['Team'] == 'Away'].index)
#        print(AwayPlayers)

        # and number of balls
        if Balls:
            BallPlayers = 1
        else:
            BallPlayers = 0

        AllPlayers = HomePlayers + AwayPlayers + BallPlayers
        # compute and plot Voronoi tessellation from player positions
        vor = Voronoi(pos_array[:,:2])

        # plot
        regions, vertices = voronoi_finite_polygons_2d(vor)

        # colourise
        teamkey = np.append(np.ones(HomePlayers)*0.9, np.ones(AwayPlayers)*0.1)
        norm = mpl.colors.Normalize(vmin=0.0, vmax=1.0, clip=True)
        mapper = cm.ScalarMappable(norm=norm, cmap=cm.coolwarm)

        for idx, region in enumerate(regions):
            polygon = vertices[region]
            plt.fill(*zip(*polygon), alpha=0.4, color = mapper.to_rgba(teamkey[idx]))

        for j in range(AllPlayers):
            if  team_frame.at[j, 'Team'] == 'Home':
                TeamColour = 'r'
                # colour smart players differently so we can identify them more easily
                if team_frame.at[j, 'Smart']:
                    TeamColour = 'm'

            elif team_frame.at[j, 'Team'] == 'Away':
                TeamColour = 'b'
                if team_frame.at[j, 'Smart']:
                    TeamColour = 'c'

            plt.plot(pos_array[j,0], pos_array[j,1], 'o', color = TeamColour)

            # write spatial control (in %) of each player near them
            plt.text(pos_array[j,0] + 0.8, pos_array[j,1] + 0.8,  '%.1f' % (100 * pos_array[j,2]), fontsize = 8)

        # plot balls if we want them
        if Balls:
            plt.plot(ball_pos[0,0], ball_pos[0,1], 'o', color = 'k')

        plt.xlim(-0.5 * PitchX, 0.5 * PitchX)
        plt.ylim(-0.5 * PitchY, 0.5 * PitchY)

        # print Voronoi plot
        # check if output folder exists
        if not exists('plots'):
            makedirs('plots')
        if not exists('plots/voronoi_plots_%s' % name):
            makedirs('plots/voronoi_plots_%s' % name)

        plt.savefig('plots/voronoi_plots_%s/voronoi-%04d.png' % (name, FrameNum + i))
        plt.clf()
