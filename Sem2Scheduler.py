from getpass import getpass
from datetime import datetime
import subprocess as s
import os
import sys
import socket

DEFAULT_PROCESSES = 4
PROCESSES = DEFAULT_PROCESSES

if socket.gethostname() == 'Lewis-Desktop':
    print('Lewis\'s Desktop: 10T (cucked by Windows)')
    PROCESSES = 10 
elif socket.gethostname() == 'ptcone30':
    print('ptcone30: 8T to abuse')
    PROCESSES = 8
else:
    print('Unknown host: 4T (potential normie hardware)')


#   schedule files are formatted as follows:
#
#   python3 task.py arg1 arg2 ... argn
#
#   to iterate a task over a list of values, replace argument with list:
#
#   python3 task.py arg1 ... [argj(1)...argj(m)] ... argn
#   
#   Note: only works for one argument per line - cannot iterate over 2 values at once (yet)

def tsplit(string, delimiters):
    # Behaves as str.split but supports multiple delimiters
    
    delimiters = tuple(delimiters)
    stack = [string,]
    
    for delimiter in delimiters:
        for i, substring in enumerate(stack):
            substack = substring.split(delimiter)
            stack.pop(i)
            for j, _substring in enumerate(substack):
                stack.insert(i+j, _substring)
            
    return(stack)


def ImportSchedule(filename):
    path = 'data_files/schedules'
    tasks = []

    # open schedule file
    with open('%s/%s.sch' % (path, filename), 'r') as f:
        lines = [line.strip('\n') for line in f.readlines()]
        
        for line in lines:
            # delete comment lines
            if line.strip().find('#') == 0:
                del line
            
            # search for square brackets
            elif line.find('[') == -1:
                # if not found, append line to tasks
                tasks.append(line.split(' '))

            else:
                # extract list of quantities
                sections = tsplit(line, ('[', ']'))
                preargs = [x for x in sections[0].split(' ') if x]
                print(sections[1])
                quants = [x.strip() for x in sections[1].split(',') if x]
                postargs = [x for x in sections[2].split(' ') if x]
                print(quants)
                # and then create list of tasks
                for quant in quants:
                    tasks.append(preargs + [quant] + postargs) 

    print(tasks)    
    return(tasks)


def RunSchedule(schedule):    
    task_data = []

    # iterate through tasks in schedule
    for task in schedule:
        dat = {
        'name': None,
        'start': None,
        'finish': None
        }

        dat['name'] = task[1]
        try:
            # run task
            dat['start'] = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            s.Popen(task).wait()
        except Exception as e:
            # task failed, print to terminal (task[1] gives name of program)
            print('Failed %s: %s\n' % (dat['name'], str(e)))
            pass
        else:
            # task finished, get finish time
            dat['finish'] = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        task_data.append(dat)
        
        print('\n')

    for dat in task_data:       
        print('Task:\t%s\nStarted:\t%s\nFinished:\t%s\n' % (dat['name'], dat['start'], dat['finish']))


def main(filename):
    schedule = ImportSchedule(filename)
    RunSchedule(schedule)
    finish_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    print('All tasks finished: %s' % finish_time)


if __name__ == '__main__':
    # sys args
    if len(sys.argv) == 2:
        filename = sys.argv[1]
    else:
        print('''
    %s - Schedules multiple programs/runs using a .sch file.
        Args:
        [1] - Filename (Schedule files must be placed in data_files/schedules/)
        ''')
        exit()

    # run main once sys args processed
    main(filename)