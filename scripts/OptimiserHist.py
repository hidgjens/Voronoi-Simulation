import pandas as pd
import numpy as np
import PeakFitting as pf
import lib.DFFilters as DFF
from lib.DFFilters import FilterData
from os import makedirs
from os.path import exists

# filter df
def filter(df, run_name):
    # apply filter settings
    filters = ['RmHomeTeam', 'RmAwayTeam', 'RmBall', 'SlSmartPlayer', 'RmStationary']
    filter_settings = [False, True, True, False, False]
    filt_dict = dict(zip(filters,filter_settings))

    # remove frame 0 as default
    df, diff1 = FilterData(df, DFF.Rm0Frame)
    print('Rm0Frame cut: %i' % diff1)

    # other filters applied based on settings
    for filt, setting in filt_dict.items():
        if setting:
            filtered, diff = FilterData(df, getattr(DFF, filt))
            print('%s cut: %i' % (filt, diff))
            df = filtered
    return(df)

# calculates histogram values (mean, stdev, ) to a file for a given value of parameter
def CalcHist(run_name, date, param):
    filename = '%s.%s' % (date, run_name)
    # import all matches from run
    df = filter(pf.LoadRunName(filename), run_name)
    
    # only take first home player per frame
    df = df.loc[(df['Team'] == 'Home') & (df['Num'] == 1)]

    # calculate histogram
    hist = pf.Histogram(df, 'TmCtrl')
    mean = hist.mean
    stdev = hist.stdev
    entries = hist.n

    return(mean, stdev, entries)

# NOT 
# NEEDS EDITING TO PULL CSVS FOR SPECIFIC P VALUES

def main(run_name, date, pmin, pmax, pstep):
    filename = '%s.%s' % (date, run_name)
    data = []
    # iterate through p values
    for param in range(pmin, pmax, pstep):
        
        # append sim values to data
        data.append(CalcHist(run_name, date, param))

    df = pd.DataFrame(data, columns = ['Mean','Stdev','Entries'])
    
    # check output folder exists
    if not exists('data_files/param_csvs/%s' % filename):
        makedirs('data_files/param_csvs/%s' % filename)

    # save to file
    df.to_csv('data_files/param_csvs/%s/%s.csv' % (filename, filename))

if __name__ == '__main__':

    # process sys args
    if len(sys.argv) == 6:
        run_name = sys.argv[1]
        date =  = sys.argv[2]
        pmin =  = sys.argv[3]
        pmax = sys.argv [4]
        pstep = sys.argv[5]
    
    else:
        print('''
        %s - Plots Voronoi diagrams (multithreaded) over matches and plots vids
        Args:
        [1] - Run Name (MatchConf:HomeTeam:AwayTeam)
        [2] - Date (DD.MM)
        [3] - Min value of parameter
        [4] - Max value of parameter
        [5] - Parameter step size
        ''' % sys.argv[0])
        exit()
    
    # now run main
    main(run_name, date, pmin, pmax, pstep)