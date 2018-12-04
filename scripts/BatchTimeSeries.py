import pandas as pd
import numpy as np
import matplotlib as mpl
mpl.use('agg')
import sys
import matplotlib.cm as cm
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from os import makedirs
from os.path import exists
from scipy import signal
from PeakFitting import filter

def getPossessionRegions(df):
    # take one player per frame
    df = df.loc[(df['Team'] == 'Home') & (df['Num'] == 1)]
    Frames = 1 + df['FID'].max()
    startpoints = []
    endpoints = []
    colours = []

    def possToColour(poss):
        if poss:
            return('r')
        elif not poss:
            return('b')
        else:
            print('Why am I here?')
            return('g')

    # get possession in first frame, add startpoint and colour
    poss = np.asscalar(df.loc[df['FID'] == 1, 'Possession'].values)
    startpoints.append(1)
    colours.append(possToColour(poss))

    # find when possession changes, add region startpoint, endpoint and colour to list, then update possession
    for i in range(1, Frames):
        frameposs = np.asscalar(df.loc[df['FID'] == i, 'Possession'].values)
        if frameposs != poss:
            startpoints.append(i)
            endpoints.append(i)
            poss = frameposs
            colours.append(possToColour(poss))

    # add final endpoint 
    endpoints.append(Frames)
    return(list(zip(startpoints, endpoints, colours)))

# computes mean of a variable in a possession region, stores as list
def possRegionMean(df, var):
    # get details of possession regions
    startpoints, endpoints, colours = zip(*getPossessionRegions(df))
    del colours

    means = []
    # compute mean of var in each region    
    for s, e in zip(startpoints, endpoints):
        regionvalues = df.loc[(df['FID'] >= s) & (df['FID'] <= e), var].values       
        regionmean = sum(regionvalues) / len(regionvalues)
        means.append(regionmean)
    
    return(means)

# plots time series graphs for a given match
def TimeSeriesPlot(df, var, title, filename):    
    # define useful quantities
    frames =  1 + df['FID'].max()
    players = df['Num'].max()
    t = range(1, frames) 
    colours = cm.plasma(np.linspace(0, 1, players)) 
    patches = []

    plt.figure(figsize = (21, 7), dpi = 300) 
    # shade background regions
    for s, e, c in getPossessionRegions(df):
        plt.axvspan(s, e, facecolor = c, alpha = 0.4)

    for i in range(players):
        # get player data
        plyrdf = df.loc[df['Num'] == i + 1]
        plyrdata = plyrdf[var].tolist()

        # plot
        plt.plot(t, plyrdata, color = colours[i], linestyle = '-', linewidth = 0.75)
        # print('Player plotted: %i' % (i + 1))

        patches.append(mpatches.Patch(color = colours[i], label = 'Player %i' % (i + 1)))

    # formatting
    plt.xlim(0,frames)
    plt.ylim(0,)
    plt.xlabel('Frame')
    plt.ylabel(var)
    plt.title(title)

    # legend
    plt.legend(handles = patches, loc = 1)

    # better safe than sorry
    if not exists('plots/time_series/%s' % filename):
        makedirs('plots/time_series/%s' % filename)

    plt.savefig('plots/time_series/%s/%s.png' % (filename, title))
    plt.close()

# plots time series of teams spatial control
def TimeTeam(df, var, title, filename):    
    # define useful quantities
    tmdata = df[var].tolist()
    frames =  1 + df['FID'].max()
    t = range(1, frames)

    plt.figure(figsize = (21, 7), dpi = 300)     
    # plot possession regions and mean dCtrl (ie mean gradient over region)
    means = possRegionMean(df, 'TmdCtrl')
    for i, regionlist in enumerate(getPossessionRegions(df)):
        s, e, c = regionlist
        height = 5*(1 + signal.sawtooth(np.pi/2 * i)) 
        plt.axvspan(s, e, facecolor = c, alpha = 0.4)
        plt.text((s+e)/2, height, "{:.2f}".format(means[i]), fontsize = 10, color = c)
    
    # plot
    plt.plot(t, tmdata, linestyle = '-', linewidth = 0.75, color = 'k')

    # formatting
    plt.xlim(0, frames)
    plt.ylim(0,)
    plt.xlabel('Frame')
    plt.ylabel(var)
    plt.title(title)

    # better safe than sorry
    try:
        if not exists('plots/time_series/%s' % filename):
            makedirs('plots/time_series/%s' % filename)
    except:
        pass


    plt.savefig('plots/time_series/%s/%s.png' % (filename, title))
    plt.close()

# plots time series graphs for all matches in a given run
def PlotMatches(df, match_nums, var, title, filename):
    # convert values to percentages
    vars = ['Ctrl', 'dCtrl', 'TmCtrl', 'TmdCtrl']
    for v in vars:
        df[v] = df[v].apply(lambda x: x*100)

    frames = df['FID'].max() + 1
    print('Frames: %s' % frames)
    # split dataframe into matches
    for i in match_nums:
        mdf = df.loc[df['MID'] == i]
        # pass match to plot
        TimeSeriesPlot(mdf, var, '%s | Match %i' % (title, i), filename)
        # filter out single player for team plot
        mdf = mdf.loc[(mdf['Team'] == 'Home') & (mdf['Num'] == 1)]
        # plot with team variable
        TimeTeam(mdf, 'Tm%s' % var, '%s | Team Plot | Match %i' % (title, i), filename)
        print('Match %i plotted.' % i)

def plotBatch(df, match_nums, run_name, date):
    filename = '%s.%s' % (date, run_name)    
    # pre-filter dataframe
    df = filter(df, run_name)
    # plot graphs
    PlotMatches(df, match_nums, 'Ctrl', '%s | Time Series' % filename, filename)