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
from scripts.ChangeConfig import config_file_directories


# parameter tuning
# save original config file to backup
def BackupConfig(config):
    # find config file directory in dictionary
    config_file_dir = config_file_directories[config]
    
    # load original config
    config_dict = ChCFG.load_config(config_file_dir, identifier='')[0]
    # save to backup
    ChCFG.save_config(config_dict, config_file_dir, identifier='-Backup')

def OverwriteConfig(config):
    # find config file directory in dictionary
    config_file_dir = config_file_directories[config]
    
    # load backup config
    config_dict = ChCFG.load_config(config_file_dir, identifier='-Backup')[0]
    # save to original
    ChCFG.save_config(config_dict, config_file_dir, identifier='')

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



def VaryParameter(strategy:str, config:str, parameter:str , min_pwr:float, max_pwr:float, N:int, num_matches:int):
    run_name:str = 'Std:%s:Random' % strategy
    results = []

    # take parameter limits and number of steps and convert to values
    step_size = (max_pwr - min_pwr)/(N-1)
    steps = [(min_pwr + n*step_size) for n in range(N)]
    # want values such that min_pwr < log10(x) < max_pwr
    vals = np.power(10.0 * np.ones(N), steps)

    # loop over each value of the parameter
    for x in vals:
        # get date of run
        date: str = '%s' % datetime.now().strftime('%d.%m')
        filename = '%s.%s' % (date, run_name)
        
        # change paramter value in config file
        identifier:str = ''
        config_file_dir = config_file_directories[config]
        ChCFG.change_config_line(config_file_dir, parameter, x, identifier)
        
        # run launcher with this config file
        s.Popen(['python3', 'Sem2Launcher.py', str(num_matches), '2000', 'conf', run_name]).wait()
  
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
    stdevs = [float(s) for s in stdevs]
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


def main(strategy:str, config:str, parameter:str , min_pwr:float, max_pwr:float, N:int, num_matches:int):
    # backup original config file
    BackupConfig(config)

    # run parameter tuning
    VaryParameter(strategy, config, parameter, min_pwr, max_pwr, N, num_matches)
    print('Finished!')

    # overwrite original config file with backup file
    OverwriteConfig(config)

if __name__ == '__main__':
    # sys args
    if len(sys.argv) == 8:
        strategy = sys.argv[1]
        config = sys.argv[2]
        parameter = sys.argv[3]
        min_pwr = float(sys.argv[4])
        max_pwr = float(sys.argv[5])
        N = int(sys.argv[6])
        num_matches = sys.argv[7]
    else:
        print('''
    %s - Plots spatial control of a strategy with respect to a changing parameter.
        Args:
        [1] - Strategy containing parameter to vary (Exchange/DSquared etc.)
        [2] - Config file to change (Exchange etc.)
        [3] - Parameter to vary (DecayCoeff etc.)
        [4] - Minimum value of paramter as exponent, eg: -3.0 -> 10^-3
        [5] - Maximum value of parameter as exponent, eg: 3.0 -> 10^3
        [6] - Number of steps between min and max values of parameter (inclusive)
        [7] - Number of matches per run
        ''' % sys.argv[0])
        exit()

    # sys args sorted, now run main
    main(strategy, config, parameter, min_pwr, max_pwr, N, num_matches)