from os import listdir, makedirs 
from typing import List
import sys

def ListSums(filename:str, cutoff:float):
    # load .sum files corresponding to run
    path:str = 'data_files/csvs/%s' % filename 
    sums:List[str] = [file for file in listdir(path) if file.split('.')[-1] == 'sum']

    # find .sums for which team control is below cutoff
    new_sums:List[str] = []
    for sum in sums:
        with open(path + '/' + sum, 'r') as f:
            homectrl:float = float(f.readline().strip('\n'))
            if homectrl < cutoff:
                new_sums.append(sum)
    
    return(new_sums)

def main(filename:str, cutoff:str):
    sums:List[str] = ListSums(filename, cutoff)
    print(sums)
    print('\nFound %i faulty games' % (len(sums)))

if __name__ == '__main__':
    # sys args
    if len(sys.argv) == 3:
        filename:str = sys.argv[1]
        cutoff:float = float(sys.argv[2])

    else:
        print('''
        %s - returns list of .sum files with home control below threshold.
        Args:
        [1] - filename (DD.MM.Match:HomeTeam:AwayTeam)
        [2] - threshold spatial control (eg. 0.35)
        ''' % sys.argv[0])
        exit()

    main(filename, cutoff)
