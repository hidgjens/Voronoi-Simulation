import sys
from numpy import sqrt

def load_script(script_file_name):
    with open(script_file_name, 'r') as script_file:
        line = script_file.readline()
    
    segments_of_line = line.split()
    
    # python3 Sem2Launcher matches frames visual conf strat1 strat2 strat3...

    # can ignore first 6 elements
    run_types = segments_of_line[6:]
    number_of_games = int(segments_of_line[2])

    return(run_types, number_of_games)

def load_summary(date_string, run_name, number_of_games):
    # print(run_name)
    run_name_split = run_name.split(':')
    # print(run_name_split)

    home_strategy = run_name_split[1]
    away_strategy = run_name_split[2]

    result_dict = {
        'home_strategy' : home_strategy,
        'home_mean' : 0.0,
        'home_mean_error' : 0.0,
        'home_std' : 0.0,
        'away_strategy' : away_strategy,
        'away_mean' : 0.0,
        'away_mean_error' : 0.0,
        'away_std' : 0.0,
        'number_of_games' : number_of_games
    }

    with open('results/summaries/%s.%s/Home.sum' % (date_string, run_name)) as home_summary:
        lines = home_summary.readlines()
        mean = float(lines[0].strip())
        std = float(lines[0].strip())

        result_dict['home_mean'] = mean
        result_dict['home_mean_error'] = std / sqrt(number_of_games)
        result_dict['home_std'] = std

    with open('results/summaries/%s.%s/Away.sum' % (date_string, run_name)) as away_summary:
        lines = away_summary.readlines()
        mean = float(lines[0].strip())
        std = float(lines[0].strip())

        result_dict['away_mean'] = mean
        result_dict['away_mean_error'] = std / sqrt(number_of_games)
        result_dict['away_std'] = std

    return(result_dict)

def main(date_string, script_name):
    run_types, number_of_games = load_script(script_name)

    list_of_result_dicts = []

    for run in run_types:
        result = load_summary(date_string, run, number_of_games)
        list_of_result_dicts.append(result)

    home_teams = []

    for result_dict in list_of_result_dicts:
        home_team = result_dict['home_strategy']
        away_team = result_dict['away_strategy']
        if home_team not in home_teams:
            home_teams.append(home_team)
        if away_team not in home_teams:
            home_teams.append(away_team)

    # home_teams will state the order of the table
    table = {}
    for home_team in home_teams:
        table[home_team] = {}
        for away_team in home_teams:
            table[home_team][away_team] = None

    for result_dict in list_of_result_dicts:
        ht = result_dict['home_strategy']
        at = result_dict['away_strategy']

        table[ht][at] = [result_dict['home_mean'], result_dict['home_mean_error']]

    for key, item in table.items():
        print(key)
        print(item)

    with open('table_file.tab', 'w') as t_file:
        # header
        t_file.write('Teams')
        
        for at in home_teams:
            t_file.write('\t%s' % at)
        t_file.write('\n')

        for ht in home_teams:
            t_file.write('%s' % ht)
            for at in home_teams:
                result = table[ht][at]
                if result is None:
                    t_file.write('\tNone')
                else:
                    t_file.write('\t%.3f,%.3f' % (result[0], result[1]))
            t_file.write('\n')



    

if __name__ == '__main__':
    arguments = ['[This Filename]', '[Date String (DD.MM)', '[Round-Robin script path (round_robin.sh)]']

    if len(sys.argv) != len(arguments):
        print('Arguments: ', *arguments[1:])
        exit()
    else:
        date_string = sys.argv[1]
        script_name = sys.argv[2]

    main(date_string, script_name)
