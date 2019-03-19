from getpass import getpass
from datetime import datetime
import subprocess
import os
import sys

import socket

DEFAULT_PROCESSES = 4
PROCESSES = DEFAULT_PROCESSES

if socket.gethostname() == 'Lewis-Desktop':
    print('Code is running on Lewis\' lightning fast (!!) desktop with 6C/12T')
    print('Unfortunately he uses Windows so he will only give 10 threads\n')
    PROCESSES = 10 
elif socket.gethostname() == 'ptcone30':
    print('Code is running on rickety-old ptcone30, classic FX-8350 with 8 cores to abuse\n')
    PROCESSES = 8
elif socket.gethostname() == 'lah-ubuntu':
    print('Lewis is using his Ubuntu partition. We have 10 threads (he wants to use it too)\n')
    PROCESSES = 10
elif socket.gethostname() == 'brendleberry-desktop':
    print('brendleberry-desktop: bentleywurst\'s ubuntu partition, 12 cores for the bois\n')
    PROCESSES = 12
else:
    print('Unknown territory: %s. Playing it safe with %i threads, edit DEFAULT_PROCESSES in %s to change this behaviour' % (socket.gethostname(), DEFAULT_PROCESSES, sys.argv[0]))


###########################################################
# # Variables to set schedule

date_str = '%s' % datetime.now().strftime('%d.%m')

print_time = {
    'cmd' : ['echo', '%s' % datetime.now().strftime('%H:%M:%S')],
    'task-name' : 'print_time_test'
}
print_date = {
    'cmd' : ['echo', '%s' % datetime.now().strftime('%d/%m')],
    'task-name' : 'print_date_test'
}

sim_types = {
    'met' : 'legacy/GenerateMetricMatches.py',
    'team' : 'legacy/GenerateTeamMatches.py',
    'conf' : 'scripts/GenerateConfMatches.py'
}



# functions to generate process dicts for match generation, histogram plotting and Voronoi analysis

def generate(strat, filename, match_num):
    frame_num = 3 # meaningless
    process_dict = {
    'cmd' : ['python3', 'scripts/GenerateConfMatches.py', '%s' % strat, '%i' % match_num, '%i' % frame_num, filename, 'no', '%i' % PROCESSES],

    'task-name' : 'Generate %s' % strat
    }
    return(process_dict)

def histogram(strat, iteration_number):
    process_dict = {
    'cmd' : ['python3', 'scripts/MetAnalysis.py', strat, '%i' % iteration_number],

    'task-name' : 'Histogram %s' % strat
    }
    return(process_dict)

def change_N(N):
    process_dict = {
        'cmd' : ['python3', 'scripts/ChangeN.py', '%i' % N],
        'task-name' : 'Changed N'
    }
    return(process_dict)

##############################################################################################################################################################################
# creates list of processes to run, in order (generates matches for all strategies, then plots all histograms, then does all visualisation)

def makeSchedule(match_num, start_num, end_num, strategies):
    schedule = []
    gen_sched = []
    hist_sched = []
    change_sched = []
    for strat in strategies:
        for i in range(start_num, end_num + 1):
            FILENAME = '%s.%i.%s' % (date_str, i, strat)
            change_sched.append(change_N(i))

            gen_sched.append(generate(strat, FILENAME, match_num))
            
            hist_sched.append(histogram(FILENAME, i))
        

    for g, h, c in zip(gen_sched, hist_sched, change_sched):
        print(g['cmd'])
        print(h['cmd'])
        print(c['cmd'])
        schedule.append(c)
        schedule.append(g)
        schedule.append(h)

    return(schedule)

##########################################################

print('##################################################')
print('####### DON\'T FORGET TO RUN ME INSIDE TMUX #######')
print('##################################################')

def get_time():
    return(datetime.now().strftime('%d/%m  %H:%M:%S'))

def runTasks(schedule):
    for tsk in schedule:
        try:
            tsk['start-time'] = get_time()
            #gen_started_message(tsk['task-name'], tsk['start-time'])
            subprocess.Popen(tsk['cmd']).wait()
        except Exception as e:
            #gen_failed_message(tsk['task-name'], tsk['start-time'], str(e))
            tsk['end-time'] = get_time()

            print('Failed %s: %s\n' % (tsk['task-name'], str(e)))
            pass
        else:
            #gen_success_message(tsk['task-name'], tsk['start-time'])
            tsk['end-time'] = get_time()
    print('\n')
    for tsk in schedule:
        print('Task:\t%s\nStarted:\t%s\nFinished:\t%s\n' % (tsk['task-name'], tsk['start-time'], tsk['end-time']))

def main(match_num, start_num, end_num, strategies):
    # builds schedule and then runs processes
    schedule = makeSchedule(match_num, start_num, end_num, strategies)
    runTasks(schedule)

if __name__ == '__main__':
    # process sys args
    if len(sys.argv) >= 5:
        match_num = int(sys.argv[1])
        start_num = int(sys.argv[2])
        end_num = int(sys.argv[3])
        strategies = sys.argv[4:]

    else:
        print('''
    %s - Generates and analyses multiple matches.
        Args:
        [1] - Number of matches to generate per type
        [2] - Starting N
        [3] - Ending N (inclusive)
        [4+] - Match types (MatchConf:HomeConf:AwayConf)
        ''' % sys.argv[0])
        exit()

    main(match_num, start_num, end_num, strategies)