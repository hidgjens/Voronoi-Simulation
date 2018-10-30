from getpass import getpass
from datetime import datetime
import subprocess
import os
import sys
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

# functions to generate process dicts for match generation, histogram plotting and Voronoi analysis

def generate(strat, match_num, frame_num):
    process_dict = {
    'cmd' : ['python3', 'GenerateTeamMatches.py', '%s' % strat, '%i' % match_num, '%i' % frame_num, '%s.%s' % (date_str, strat), 'no', '8'],

    'task-name' : 'Generate %s' % strat
    }
    return(process_dict)

def histogram(strat):
    process_dict = {
    'cmd' : ['python3', 'HistPlot.py', strat, date_str],

    'task-name' : 'Plot %s' % strat
    }
    return(process_dict)

def voronois(strat, vid_num):
    process_dict = {
    'cmd' : ['python3', 'scripts/VorVids.py', '%s.%s' % (date_str, strat), str(vid_num)],

    'task-name' : 'Voronois %s' % strat
    }
    return(process_dict)

##############################################################################################################################################################################
# creates list of processes to run, in order (generates matches for all strategies, then plots all histograms, then does all visualisation)

def makeSchedule(match_num, frame_num, vid_num, strategies):
    schedule = []
    gen_sched = []
    hist_sched = []
    vor_sched = []

    for strat in strategies:
        gen_sched.append(generate(strat, match_num, frame_num))
        hist_sched.append(histogram(strat))
        vor_sched.append(voronois(strat, vid_num))

    schedule = gen_sched + hist_sched + vor_sched

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

def main(match_num, frame_num, vid_num, strategies):
    # builds schedule and then runs processes
    schedule = makeSchedule(match_num, frame_num, vid_num, strategies)
    runTasks(schedule)

if __name__ == '__main__':
    # process sys args
    if len(sys.argv) >= 4:
        match_num = int(sys.argv[1])
        frame_num = int(sys.argv[2])
        vid_num = int(sys.argv[3])
        strategies = sys.argv[4:]
    else:
        print('''
    %s - Generates and analyses multiple matches.
        Args:
        [1] - Number of matches to generate per type
        [2] - Frames per match
        [3] - Number of matches to visualise
        [4+] - Match types (HomeTeam:smrtcnt:AwayTeam:smrtcnt)
        ''' % sys.argv[0])
        exit()

    main(match_num, frame_num, vid_num, strategies)
