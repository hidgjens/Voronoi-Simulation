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
    'cmd' : ['python3', 'GenerateMatches.py', '%s' % strat, '%i' % match_num, '%i' % frame_num, '%s.%s' % (date_str, strat), 'no', '8'],

    'task-name' : 'Generate %s' % strat
    }
    return(process_dict)

def histogram(strat):
    process_dict = {
    'cmd' : ['python3', 'scripts/FilterSimData.py', '%s.%s' % (date_str, strat), '30', '%s.%s' % (date_str, strat)],

    'task-name' : 'Plot %s' % strat
    }
    return(process_dict)

def voronois(strat, vid_num):
    process_dict = {
    'cmd' : ['python3', 'scripts/VorVids.py', '%s.%s' % (date_str, strat), '%i' % vid_num],

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

def upload_message(path):
    name = path.split('/')[-1]
    srv = pysftp.Connection(
        host = '192.168.1.17',
        username = 'lewis',
        password = '')
    srv.chdir('/home/lewis')
    srv.put(path)
    os.remove(path)
    return('/home/lewis/%s' % name)

def sendemail(remote_file, subject = 'ptcone30 update'):
    client = paramiko.SSHClient()
    client.load_system_host_keys()
    client.set_missing_host_key_policy(paramiko.WarningPolicy)
    client.connect('192.168.1.17', username='lewis', password=passwd)
    ssh_stdin, ssh_stdout, ssh_stderr = client.exec_command('''sendemail -l email.log \
    	-f "LewisRPi@ptcone30" \
    	-u "%s" \
    	-t "lewis.higgins97@hotmail.com;jimworsfold@msn.com" \
    	-s "smtp.gmail.com:587" \
    	-o tls=yes \
    	-xu "safarimanow@gmail.com" \
    	-xp "%s" \
    	-o message-file="%s"''' % (subject ,'', remote_file))

    #client.exec_command('rm %s' % remote_file)
    client.close()

def gen_started_message(task, start_time, subject=''):
    message = ['ptcone30', 'Started task %s' % task, 'Time started: %s' % start_time]
    with open('%s_started.msg' % task, 'w') as msg:
        for line in message:
            msg.write('%s\n' % line)
    if subject == '':
        subject = 'ptcone30: %s started' % task
    remote_msg = upload_message('%s_started.msg' % task)
    sendemail(remote_msg, subject)

def gen_success_message(task, start_time, subject=''):
    finish_time = get_time()
    message = ['ptcone30', 'Task %s has finished' % task, 'Time started: %s' % start_time, 'Time complete: %s' % finish_time]
    with open('%s_success.msg' % task, 'w') as msg:
        for line in message:
            msg.write('%s\n' % line)
    if subject == '':
        subject = 'ptcone30: %s completed' % task
    remote_msg = upload_message('%s_success.msg' % task)
    sendemail(remote_msg, subject)

def gen_failed_message(task, start_time, error_msg = '', subject = ''):
    finish_time = get_time()
    message = ['ptcone30', 'Task %s has failed' % task, 'Time started: %s' % start_time, 'Time stopped: %s' % finish_time, 'Error message: %s' % error_msg]
    with open('%s_failed.msg' % task, 'w') as msg:
        for line in message:
            msg.write('%s\n' % line)
    if subject == '':
        subject = 'ptcone30: %s failed' % task
    remote_msg = upload_message('%s_failed.msg' % task)
    sendemail(remote_msg, subject)

def runTasks(schedule):
    for tsk in schedule:
        try:
            tsk['start-time'] = get_time()
            #gen_started_message(tsk['task-name'], tsk['start-time'])
            subprocess.Popen(tsk['cmd']).wait()
        except Exception as e:
            #gen_failed_message(tsk['task-name'], tsk['start-time'], str(e))
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
    %s - Generates and analyses multiple matches
        args:
        [1] - Number of matches to generate
        [2] - Frames per match
        [3] - Number of matches to visualise
        [4+] - Scenarios to use (1HExchange, etc.)
        ''' % sys.argv[0])
        exit()

    main(match_num, frame_num, vid_num, strategies)