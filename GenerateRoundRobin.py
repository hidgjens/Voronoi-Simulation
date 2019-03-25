# bit sloppy on this, but whatever
import sys

strategies = [
    'Tether',
    'TetherSpreading',
    'TetherElastic',
    'TetherGrid',
    'TetherMetricN',
    'MetricN',
    'Exchange',
    'Spreading',
    'Random'
    ]

boring_strategies = ['Random', 'Exchange', 'Spreading']

if len(sys.argv) != 2:
    print('Enter number of matches per matchup')
    exit()
else:
    try:
        number_of_matches = int(sys.argv[1])
    except Exception as e:
        print('You probably didn\'t type an integer: %s (%s)' %(sys.argv[1], str(e)))
        exit()


strategy_codes = []
    
for idx, strategy1 in enumerate(strategies):
    for strategy2 in strategies[idx + 1:]: # +1 removes mirror matchups
        if (strategy1 in boring_strategies) and (strategy2 in boring_strategies):
            continue
        strat_string = 'Std:%s:%s' % (strategy1, strategy2)
        strategy_codes.append(strat_string)

launch_string = 'python3 Sem2Launcher.py %i 2 0 conf' % number_of_matches

for c in strategy_codes:
    launch_string += ' %s' % c

print(launch_string)

number_of_matchups = len(strategy_codes)

print('\n############################################################')
print('Matchups: %i' % number_of_matchups)
print('Games per matchup: %i' % number_of_matches)
print('Total number of games: %i' % (number_of_matches * number_of_matchups))
print('############################################################')


with open('round_robin.sh', 'w') as rr:
    rr.write(launch_string + '\n')

