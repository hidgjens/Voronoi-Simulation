from getpass import getpass
from datetime import datetime
import subprocess
import os
###########################################################
# # Variables to set schedule
# process dicts (don't edit these, take copies if you want to alter them):
date_str = '%s' % datetime.now().strftime('%d.%m')

print_time = {
    'cmd' : ['echo', '%s' % datetime.now().strftime('%H:%M:%S')],
    'task-name' : 'print_time_test'
}
print_date = {
    'cmd' : ['echo', '%s' % datetime.now().strftime('%d/%m')],
    'task-name' : 'print_date_test'
}
generate_1HMetExch = {
    'cmd' : ['python3', 'GenerateMatches.py', '1HMetExch', '5000', '100', '%s.1HMetExch' % date_str, 'no', '8'],
    'task-name' : 'Generate 1HMetExch'
}
histogram_1HMetExch = {
    'cmd' : ['python3', 'scripts/FilterSimData.py', '%s.1HMetExch' % date_str, '30', '%s.1HMetExch' % date_str],
    'task-name' : 'Plot 1HMetExch'
}
generate_1HRandomWalk = {
    'cmd' : ['python3', 'GenerateMatches.py', '1HRandomWalk', '5000', '100', '%s.1HRandomWalk' % date_str, 'no', '8'],
    'task-name' : 'Generate 1HRandomWalk'
}
histogram_1HRandomWalk = {
    'cmd' : ['python3', 'scripts/FilterSimData.py', '%s.1HRandomWalk' % date_str, '30', '%s.1HRandomWalk' % date_str],
    'task-name' : 'Plot 1HRandomWalk'
}
generate_RandomWalk = {
    'cmd' : ['python3', 'GenerateMatches.py', 'RandomWalk', '5000', '100', '%s.RandomWalk' % date_str, 'no', '8'],
    'task-name' : 'Generate RandomWalk'
}
histogram_RandomWalk = {
    'cmd' : ['python3', 'scripts/FilterSimData.py', '%s.RandomWalk' % date_str, '30', '%s.RandomWalk' % date_str],
    'task-name' : 'Plot RandomWalk'
}
generate_1HExchange = {
    'cmd' : ['python3', 'GenerateMatches.py', '1HExchange', '5000', '100', '%s.1HExchange' % date_str, 'no', '8'],
    'task-name' : 'Generate 1HExchange'
}
histogram_1HExchange = {
    'cmd' : ['python3', 'scripts/FilterSimData.py', '%s.1HExchange' % date_str, '30', '%s.1HExchange' % date_str],
    'task-name' : 'Generate 1HExchange'
}


##############################################################################################################################################################################
# list of processes to run (in order, add the dict for desired process)
schedule = [
    generate_RandomWalk,
    histogram_RandomWalk,
    generate_1HExchange,
    histogram_1HExchange,
    generate_1HRandomWalk,
    histogram_1HRandomWalk
]
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
