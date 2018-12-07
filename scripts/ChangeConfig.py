config_file_directories = {
    'Std' : 'matches/Std',
    'Lin' : 'matches/Lin',
    'Step' : 'matches/Step',
    'FourStep' : 'matches/FourStep',
    'DSquared' : 'parameters/DSquared',
    'Exchange' : 'parameters/Exchange'
}

import sys

def print_dict(dictionary):
    for key, value in dictionary.items():
        print('\t%s:\t%s' % (key, value))

def load_config(config_file_dir):
    # open config file which we want to write to

    config_dict = {}
    ordered_list = [] # just to put the config back in the same order

    with open('configs/%s.cfg' % config_file_dir, 'r') as config_file:
        
        # load lines into dict
        for line in config_file.readlines():
            l = line.split(':') 
            config_dict[l[0]] = l[1].strip()
            ordered_list.append(l[0])

    return(config_dict, ordered_list) # returns config as a dictionary and list with the order they were written in (dicts are unordered maps)

def save_config(config_dict, config_file_dir, fields = None): # fields is a list of the dict keys you want to save and in what order, otherwise any order is chosen


    with open('configs/%s.cfg' % config_file_dir, 'w') as config_file:
        if fields is None:
            # no order given, just go with whatever order
            for key, value in config_dict.items():
                config_file.write('%s:%s\n' % (key, value))
            #
        else:
            # use order given
            for key in fields:
                config_file.write('%s:%s\n' % (key, config_dict[key]))

def check_config_dict(config_file_dir):
    if config_file_dir in config_file_directories.keys():
        dir = config_file_directories[config_file_dir]
            
        print('Key \'%s\' found pointing to %s:\n' % (config_file_dir, dir))     

    else:
        print('Entry \'%s\' not found, trying dir = %s:\n' % (config_file_dir, config_file_dir))

        dir = config_file_dir

    return dir


def change_config_line(config_file_dir, line_name, new_value): 
    # config file must also contain any subfolders. This script checks 
    # configs/config_file

    # config file might be a dict key, check this:
    config_file_dir = check_config_dict(config_file_dir)

    # open config file which we want to write to

    config_dict, ordered_list = load_config(config_file_dir)

    # print the config file in its current state
    print('%s has loaded as:\n' % config_file_dir)
    print_dict(config_dict)

    # check requested field is in config
    if not line_name in config_dict.keys():
        raise ValueError('%s not in configs/%s.cfg' % (line_name, config_file_dir))

    # save old value for text output
    old_value = config_dict[line_name]

    if type(new_value) is float: # floats will not be converted to strings implicitly (i.e. via str()), so i give it in standard form to 5 dp
        config_dict[line_name] = '.5e' % new_value
    else:
        config_dict[line_name] = str(new_value)

    # print altered config
    print('\nSaving %s as:\n' % config_file_dir)
    print_dict(config_dict)
    print('\nwith changes:\n\t%s:%s ---> %s:%s' % (line_name, old_value, line_name, new_value))

    save_config(config_dict, config_file_dir, ordered_list)

if __name__ == '__main__':
    ARGS_REQUIRED = 3
    ARGS_PROVIDED = len(sys.argv) - 1
    # arguments needed:
    #   scripts name (or key from dict at top)
    #   line to change
    #   new value
    if len(sys.argv) == 1:
        print('''
        Args:
            [1] - Script name or key (from dict at top of %s)
            [2] - line name to change
            [3] - new value
        ''' % sys.argv[0])
    elif not len(sys.argv) == ARGS_REQUIRED + 1:
        print(''' 
    Wrong number of args.
        Args:
            [1] - Script name or key (from dict at top of %s)
            [2] - line name to change
            [3] - new value
        ''' % sys.argv[0])
    else:
        line_to_change = sys.argv[2]
        new_val = sys.argv[3]

        if sys.argv[1] in config_file_directories.keys():
            dir = config_file_directories[sys.argv[1]]
            
            print('Key \'%s\' found pointing to %s:\n' % (sys.argv[1], dir))
            

        else:
            print('Entry \'%s\' not found, trying dir = %s:\n' % (sys.argv[1], sys.argv[1]))

            dir = sys.argv[1]

        change_config_line(dir, line_to_change, new_val)
        
