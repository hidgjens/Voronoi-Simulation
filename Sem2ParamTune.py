import scripts.ChangeConfig as ChCFG
import matplotlib as mpl
mpl.use('agg')
import matplotlib.pyplot as plt
import numpy as np
import subprocess as s
import sys
from os import makedirs
from os.path import exists
from datetime import datetime

# default config file directory
config_dir = 'teams'

# parameter tuning
# save original config file to backup
def BackupConfig(config_dir, config_file):
    # load original config
    config_dict, ordered_list = ChCFG.LoadConfig(config_dir, config_file)
    # save to backup file
    ChCFG.SaveConfig(config_dir, '%s-Backup' % config_file, config_dict, ordered_list)

def OverwriteConfig(config_dir, config_file):
    # load backup config
    config_dict, ordered_list = ChCFG.LoadConfig(config_dir, '%s-Backup' % config_file)
    # save to original
    ChCFG.SaveConfig(config_dir, config_file, config_dict, ordered_list)

# save error bar plot
def PlotErrorBar(x, y, e, parameter:str, filename:str):
    try:
        plt.clf()
    except:
        print('except')
        
    plt.errorbar(x, y, e, linestyle = '', marker = 'x', c = 'k', ecolor = 'r')
    plt.title('%s\nVarying %s' % (filename, parameter))
    plt.ylabel('Mean Team Spatial Control')
    plt.xlabel('log(%s)' % parameter)

    # save fig
    path = 'plots/param-opts/%s' % filename
    if not exists(path):
        makedirs(path)

    plt.savefig('%s/%s.png' % (path, parameter))



def VaryParameter(run_name:str, config_file:str, parameter:str , min_pwr:float, max_pwr:float, N:int, num_matches:int):
    results = []

    # take parameter limits and number of steps and convert to values
    step_size = (max_pwr - min_pwr)/(N-1)
    steps = [(min_pwr + n*step_size) for n in range(N)]
    # want values such that min_pwr < log10(x) < max_pwr
    vals = np.power(10.0 * np.ones(N), steps)
    
    #vals = list(range(int(min_pwr), int(max_pwr)+1))
    #steps = vals

    # loop over each value of the parameter
    for x, i in enumerate(vals):
        # get date of run
        date: str = '%s' % datetime.now().strftime('%d.%m')
        filename = '%s.%s' % (date, run_name)
        
        # change paramter value in config file
        ChCFG.ChangeConfigLine(config_dir, config_file, parameter, x)
        
        # run launcher with this config file
        s.Popen(['python3', 'Sem2Launcher.py', str(num_matches), '2000', '0', 'conf', run_name]).wait()
  
        # load results
        path = 'results/summaries/%s.%s' % (date, run_name)
        results_file = '%s/Home.sum' % path
        
        with open(results_file, 'r') as f:
            lines = f.readlines()
            lines = [line.strip('\n') for line in lines]
            # append results to list
            results.append(lines)

    # plot errorbar plot using mean and stdev
    means, stdevs = zip(*results)
    means = [float(m) for m in means]
    stdevs = [float(s)/np.sqrt(num_matches) for s in stdevs]
    steps = [float(s) for s in steps]
    PlotErrorBar(steps, means, stdevs, parameter, filename)

    # save results to file
    path2 = 'results/param_opts' 
    points = zip(steps, means, stdevs)
    if not exists(path2):
        makedirs(path2)

    with open('%s/%s.txt' % (path2, filename), 'w') as f:
        # file header
        f.write('# Parameter tuning results, formatted as: (x,y,e)\n')
        f.write('# x: parameter, y: spatial control, e: standard deviation\n')
        for point in points:
            f.write('%s\n' % str(point))


def main(run_name:str, config_file:str, parameter:str , min_pwr:float, max_pwr:float, N:int, num_matches:int):
    
    # backup original config file
    BackupConfig(config_dir, config_file)

    # run parameter tuning
    VaryParameter(run_name, config_file, parameter, min_pwr, max_pwr, N, num_matches)
    print('Finished!')

    # overwrite original config file with backup file
    OverwriteConfig(config_dir, config_file)

if __name__ == '__main__':
    # sys args
    if len(sys.argv) == 8:
        run_name = sys.argv[1]
        config_file = sys.argv[2]
        parameter = sys.argv[3]
        min_pwr = float(sys.argv[4])
        max_pwr = float(sys.argv[5])
        N = int(sys.argv[6])
        num_matches = int(sys.argv[7])
    else:
        print('''
    %s - Plots spatial control of a strategy with respect to a changing parameter.
        Args:
        [1] - Run name to optimise (MatchType:HomeTeam:AwayTeam)
        [2] - Config file to modify (Exchange, Tether, etc.)
        [3] - Parameter to vary (DecayCoeff, AttrCoeff, PostDistance, etc.)
        [4] - Minimum value of log10(parameter), ie: -3.0 = 10^-3
        [5] - Maximum value of log10(parameter), ie: 3.0 = 10^3
        [6] - Number of increments from min to max value, inclusive
        [7] - Number of matches per run
        ''' % sys.argv[0])
        exit()

    # sys args sorted, now run main
    main(run_name, config_file, parameter, min_pwr, max_pwr, N, num_matches)