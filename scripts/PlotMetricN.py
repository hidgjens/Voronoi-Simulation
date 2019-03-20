import numpy as np
import matplotlib as mpl
mpl.use('agg')
from matplotlib import pyplot as plt

import sys
from os import makedirs, listdir
from os.path import exists

def load_file(N, date_str, run_name):
    name = '%s.%i.%s' % (date_str, N, run_name)
    with open('results/MetricN/%s/%s.mnr' % (name, name), 'r') as dfile:
        lines = dfile.readlines()
    
    mean = float(lines[0].strip())
    if lines[1].strip() == 'nan':
        err = 0
    else:
        err = float(lines[1].strip())

    samples = int(lines[2].strip())

    return(mean, err, samples)


def main(start_N, end_N, date_str, run_name):
    Xs = []
    Ys = []
    eYs = []
    name = '%s.%s' % (date_str, run_name)

    for N in range(start_N, end_N + 1):
        mean, std, samples = load_file(N, date_str, run_name)
        Xs.append(N)
        Ys.append(mean)
        eYs.append(std)

    plt.errorbar(Xs, Ys, eYs, ecolor='r', elinewidth=1, capsize=5, capthick=1, marker='x', linestyle='')
    plt.xlabel('Number of players considered by MetricN')
    plt.ylabel('Mean proportion of Allied Players')
    plt.title('Metric N - %s' % run_name)
    if not exists('plots/MetricN/%s' % name):
        makedirs('plots/MetricN/%s' % name)
        
    plt.savefig('plots/MetricN/%s/MetricN.png' % name)

if __name__ == '__main__':
    if len(sys.argv) == 1:
        print('''%s - Plot MetricN results for player distributions
        
        Args:
            [1] - Start N
            [2] - Finish N (inclusive)
            [3] - Date ('DD.MM')
            [4] - RunName ('MATCH:HOME:AWAY') 
    ''' % sys.argv[0])
    elif len(sys.argv) == 5:
        start_N = int(sys.argv[1])
        end_N = int(sys.argv[2])
        date_str = sys.argv[3]
        run_name = sys.argv[4]
    else:
        print('''Arguments wrong, try again (try without arguments for help)''')

    main(start_N, end_N, date_str, run_name)
