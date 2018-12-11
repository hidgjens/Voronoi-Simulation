import subprocess as s
import sys
import matplotlib as mpl
mpl.use('agg')
from datetime import datetime
import numpy as np
import pandas as pd
import scripts.PeakFitting as pf
import scripts.lib.DFFilters as DFF
import scripts.ChangeConfig as ChCFG
import matplotlib.pyplot as plt

from scripts.lib.DFFilters import FilterData
from os import makedirs
from os.path import exists


# filter df
def filter(df, run_name: str):
    # apply filter settings
    filters = ['Rm0Frame', 'RmAwayTeam', 'RmBall']

    for filt in filters:
        filtered, diff = FilterData(df, getattr(DFF, filt))
        print('%s cut: %i' % (filt, diff))
        df = filtered
    return(df)


# calculates histogram values (mean, stdev)
def CalcHist(run_name: str, date: str):
    filename = '%s.%s' % (date, run_name)
    # import all matches from run
    df = filter(pf.LoadRunName(filename), run_name)
    
    # only take first home player per frame
    df = df.loc[(df['Team'] == 'Home') & (df['Num'] == 1)]

    # calculate histogram
    hist = pf.Histogram(df, 'TmCtrl')
    mean = hist.mean
    stdev = hist.stdev

    return(mean, stdev)


def plotScatter(results, parameter:str, run_name:str):
    # plot scatter of parameter vs mean
    formatted_results = list(zip(*results))
    xs = [np.log10(f) for f in formatted_results[0]]; means = formatted_results[1]; stds = formatted_results[2]
    #print(len(xs), len(means), len(stds))
    try:
        plt.clf()
    except:
        print('except')
    plt.errorbar(xs, means, stds, linestyle = '', marker = 'x', c = 'k', ecolor = 'r')
    plt.title('%s\nVarying %s' % (run_name, parameter))
    plt.ylabel('Team Control Distribution Mean')
    plt.xlabel('log_{10}(%s)' % parameter)

    # save
    if not exists('plots/param-opts'):
        makedirs('plots/param-opts')
    if not exists('plots/param-opts/%s' % parameter):
        makedirs('plots/param-opts/%s' % parameter)

    plt.savefig('plots/param-opts/%s/%s.png' % (parameter, run_name))


def VaryParam(config_file: str, parameter: str, run_name: str, lower_power: float, upper_power: float, steps: int):
    # vary value of parameter (X) s.t. lower_power <= log_10(X) <= upper_power
    power_ranges = np.linspace(lower_power, upper_power, steps)

    X = np.power(10.0 * np.ones(steps), power_ranges)

    results = [] # list of lists, going to append a list of [parameter, mean, std]

    for x in X:
        # get date of run
        date: str = '%s' % datetime.now().strftime('%d.%m')
        
        # x is the new parameter, need to update config.
        ChCFG.change_config_line(config_file, parameter, x)
        # now run sim
        s.Popen(['python3', 'scripts/GenerateConfMatches.py', '%s' % run_name, '200', '111', '%s.%s' % (date, run_name), 'no', '10']).wait()

  
        # get histogram results
        mean, std = CalcHist(run_name, date)
        results.append([x, mean, std])

    plotScatter(results, parameter, run_name)

if __name__ == '__main__':
    # required arguments
    # [1] config file to change (e.g. parameters/Exchange [without .cfg extension])
    # [2] parameter within config file to change
    # [3] lower power [default -3]
    # [4] upper power [default +3]
    # [5] steps [default 10]

    ARGS_REQUIRED = 3
    ARGS_MAX = 6
    ARGS_PROVIDED = len(sys.argv) - 1

    print(ARGS_PROVIDED)

    if ARGS_PROVIDED == 0:
        print('''%s - 
        ARGS:
            # [1] config file to change (e.g. parameters/Exchange [without .cfg extension])
            # [2] parameter within config file to change
            # [3] run name (Match:Team:Team)
            # [4] lower power [default -3] <- { provide
            # [5] upper power [default +3] <- { together
            # [6] steps [default 10] ''' % sys.argv[0])
    elif not (ARGS_PROVIDED >= ARGS_REQUIRED and ARGS_PROVIDED <= ARGS_MAX):
        print('''Wrong number of arguments | %s - 
        ARGS:
            # [1] config file to change (e.g. parameters/Exchange [without .cfg extension])
            # [2] parameter within config file to change
            # [3] run name (Match:Team:Team)
            # [4] lower power [default -3] <- { provide
            # [5] upper power [default +3] <- { together
            # [6] steps [default 10] ''' % sys.argv[0])
    else:
        # args
        config_file_name: str = sys.argv[1]
        field_to_change: str = sys.argv[2]
        run_name: str = sys.argv[3]
        lower_power: float = -3.0
        upper_power: float = +3.0
        steps: int = 25

        # check if optional arguments were provided

            
        if ARGS_PROVIDED >= ARGS_REQUIRED + 2:
            try:
                lower_power = float(sys.argv[ARGS_REQUIRED + 1])
                upper_power = float(sys.argv[ARGS_REQUIRED + 2])
                assert lower_power < upper_power

            except Exception as e:
                print('''Lower and Upper power must be castable to float, you typed: (%s, %s) [also put them the right way round]''' % (sys.argv[ARGS_REQUIRED + 1], sys.argv[ARGS_REQUIRED + 2]))
                exit()

        if ARGS_PROVIDED >= ARGS_PROVIDED + 3:
            try:
                steps = int(sys.argv[ARGS_REQUIRED + 3])
            except Exception as e:
                print('''Number of steps provided could not be cast to int, you typed: %s''' % (sys.argv[ARGS_REQUIRED + 3]))
                exit()                  

        VaryParam(config_file_name, field_to_change, run_name, lower_power, upper_power, steps)
        
