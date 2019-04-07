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
elif socket.gethostname() == 'lah-ThinkPad-X220':
    print('testing on laptop')
    PROCESSES = 2
elif socket.gethostname() == 'pc2014.hep.manchester.ac.uk'
    print('PC2014')
    PROCESSES = 24
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

def generate(strat, match_num, frame_num, sim_type = 'team'):
    process_dict = {
    'cmd' : ['python3.6', sim_types[sim_type], '%s' % strat, '%i' % match_num, '%i' % frame_num, '%s.%s' % (date_str, strat), 'no', '%i' % PROCESSES],

    'task-name' : 'Generate %s' % strat
    }
    return(process_dict)

def histogram(strat):
    process_dict = {
    'cmd' : ['python3.6', 'scripts/Sem2MatchAnalysis.py', strat, date_str],

    'task-name' : 'Histogram %s' % strat
    }
    return(process_dict)

def time_series(strat):
    process_dict = {
    'cmd' : ['python3.6', 'scripts/TimeSeriesMT.py', strat, date_str],

    'task-name' : 'Time series %s' % strat
    }
    return(process_dict)

def voronois(strat, vid_num):
    process_dict = {
    'cmd' : ['python3.6', 'scripts/VoronoiAnimation.py', '%s.%s' % (date_str, strat), str(vid_num)],

    'task-name' : 'Voronois %s' % strat
    }
    return(process_dict)

##############################################################################################################################################################################
# creates list of processes to run, in order (generates matches for all strategies, then plots all histograms, then does all visualisation)

def makeSchedule(match_num, frame_num, vid_num, sim_type, strategies):
    schedule = []
    gen_sched = []
    hist_sched = []
    time_sched = []
    vor_sched = []

    for strat in strategies:
        gen_sched.append(generate(strat, match_num, frame_num, sim_type))
        hist_sched.append(histogram(strat))
        vor_sched.append(voronois(strat, vid_num))
        #time_sched.append(time_series(strat))
        

    schedule = gen_sched + hist_sched + time_sched + vor_sched

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
            print('Failed %s: %s\n' % (tsk['task-name'], str(e)))
            pass
        else:
            #gen_success_message(tsk['task-name'], tsk['start-time'])
            tsk['end-time'] = get_time()
    print('\n')
    for tsk in schedule:
        print('Task:\t%s\nStarted:\t%s\nFinished:\t%s\n' % (tsk['task-name'], tsk['start-time'], tsk['end-time']))

def main(match_num, frame_num, vid_num, sim_type, strategies):
    # builds schedule and then runs processes
    schedule = makeSchedule(match_num, frame_num, vid_num, sim_type, strategies)
    runTasks(schedule)

if __name__ == '__main__':
    # process sys args
    if len(sys.argv) >= 6:
        match_num = int(sys.argv[1])
        frame_num = int(sys.argv[2])
        vid_num = int(sys.argv[3])
        sim_type = sys.argv[4]
        strategies = sys.argv[5:]

    else:
        print('''
    %s - Generates and analyses multiple matches.
        Args:
        [1] - Number of matches to generate per type
        [2] - Frames per match
        [3] - Number of matches to visualise
        [4] - Sim Type (team/met/conf)
        [5+] - Match types (HomeTeam:SmrtCnt:AwayTeam:SmrtCnt)/(Decay:Opp:Same)/(MatchConf:HomeConf:AwayConf)
        ''' % sys.argv[0])
        exit()

    main(match_num, frame_num, vid_num, sim_type, strategies)
