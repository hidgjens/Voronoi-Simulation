import numpy as np
import pandas as pd
import sys
from pathlib import Path
from os import makedirs
from os.path import exists

def generateCtrlArray(name):
    PitchX = 105; PitchY = 68

    df = pd.DataFrame(np.random.rand(PitchX, PitchY))

    # check if output folder exists
    p = Path('.').resolve().parent
    path = '%s/data_files/heatmaps' % p
    
    if not exists(path):
        makedirs(path)

    df.to_csv("%s/%s.csv" % (path, name), index = False)

def main(name):
    generateCtrlArray(name)
    
if __name__ == '__main__':
    # process sys args
    if len(sys.argv) == 2:
        name = sys.argv[1]
    else:
        print('''
        Generates random array to turn into heatmap.
        Args:
        [1] - Name of csv to generate
        ''')
        exit()
    
    main(name)