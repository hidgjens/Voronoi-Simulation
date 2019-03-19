import ChangeConfig as ChCFG
import sys

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print('provide new N (integer)')
        exit()

    new_N = int(sys.argv[1])
    ChCFG.ChangeConfigLine('teams', 'MetricN', 'PlayersToSum', new_N)
    print('done!')

