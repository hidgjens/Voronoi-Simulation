from Frame import Frame
from Pitch import Pitch

import matplotlib as mpl
from matplotlib import pyplot as plt
import matplotlib.cm as cm

from scipy.spatial import Voronoi, voronoi_plot_2d
import numpy as np
import time

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

class Animator:
    fig = None
    ax = None

    pitch = None

    def __init__(self, pitch=Pitch()):
        self.pitch = pitch
        plt.ion()

        self.fig = plt.figure()
        self.ax = self.fig.add_subplot(111)

    def plot_frame(self, frame : Frame):
        frame = frame[frame['Team'] != 'Ball'].reset_index()
        # get array of player positions and possession value
        pos_array = frame.home_player_positions.extend(frame.away_player_positions)
        poss = frame.possession

        # get number of players in frame
        HomePlayers = len(frame.home_player_positions)
        AwayPlayers = len(frame.away_player_positions)
        
        
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
            self.ax.fill(*zip(*polygon), alpha=0.4, color = mapper.to_rgba(teamkey[idx]))

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
                
            self.ax.plot(pos_array[j,0], pos_array[j,1], 'o', color = TeamColour)

            # write spatial control (in %) of each player near them
            self.ax.text(pos_array[j,0] + 0.8, pos_array[j,1] + 0.8,  '%.1f' % (100 * pos_array[j,2]), fontsize = 8)

            # write current possession state on plot
            if poss:
                posstext = 'Possession: Home'
                posscolour = 'r'
            else:
                posstext = 'Possession: Away'
                posscolour = 'b'
            self.ax.text((-self.pitch.pitch_x/2)+1, (self.pitch.pitch_y/2)+1, posstext, fontsize = 12, color = posscolour)

        self.ax.set_aspect('equal')
        self.ax.set_xlim(-0.5 * self.pitch.pitch_x, 0.5 * self.pitch.pitch_x)
        self.ax.set_ylim(-0.5 * self.pitch.pitch_y, 0.5 * self.pitch.pitch_y)
