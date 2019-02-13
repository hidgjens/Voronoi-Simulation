import sys
from os.path import exists


def PrintDict(dictionary):
    for key, value in dictionary.items():
        print('\t%s:\t%s' % (key, value))


def CheckConfigDir(config_dir):
    if not exists(config_dir):
        print('Error: config directory \'%s\' not found! Exiting now...' % config_dir)         
        exit()
    else:
        print('Config directory \'%s\' found!' % config_dir)
        return(config_dir)

# open config file which we want to write to
def LoadConfig(config_dir, config_file):
    config_dict = {}
    # just to put the config back in the same order
    ordered_list = []

    with open('configs/%s/%s.cfg' % (config_dir, config_file), 'r') as cfg:
        # load lines into dict
        for line in cfg.readlines():
            l = line.split('=') 
            config_dict[l[0]] = l[1].strip()
            ordered_list.append(l[0])
    # returns config as dict and list with order written (dicts are unordered maps)
    return(config_dict, ordered_list)

# save config file in order (or any order by default)
def SaveConfig(config_dir, config_file, config_dict, ordered_list = None):
    
    with open('configs/%s/%s.cfg' % (config_dir, config_file), 'w') as cfg:
        if ordered_list is None:
            # no order given, just go with whatever order
            for key, value in config_dict.items():
                cfg.write('%s=%s\n' % (key, value))

        else:
            # use order given
            for key in ordered_list:
                cfg.write('%s=%s\n' % (key, config_dict[key]))


def ChangeConfigLine(config_dir, config_file, line_name, new_value):
    # open config file which we want to write to
    config_dict, ordered_list = LoadConfig(config_dir, config_file)

    # print the config file in its current state
    print('%s has loaded as:\n' % config_file)
    PrintDict(config_dict)

    # check requested field is in config
    if not line_name in config_dict.keys():
        raise ValueError('%s not in configs/%s.cfg' % (line_name, config_dir))

    # save old value for text output
    old_value = config_dict[line_name]

    if type(new_value) is float: # floats will not be converted to strings implicitly (i.e. via str()), so i give it in standard form to 5 dp
        config_dict[line_name] = '.5e' % new_value
    else:
        config_dict[line_name] = str(new_value)

    # print altered config
    print('\nSaving %s as:\n' % config_file)
    PrintDict(config_dict)
    print('\nwith changes:\n\t%s:%s ---> %s:%s' % (line_name, old_value, line_name, new_value))

    SaveConfig(config_dir, config_file, config_dict, ordered_list)


if __name__ == '__main__':
    # arguments needed:
    #   config file directory
    #   config file name
    #   line to change
    #   new value

    if len(sys.argv) == 5:
        config_dir = CheckConfigDir(sys.argv[1])
        config_file = sys.argv[2]
        line_name = sys.argv[3]
        new_value = sys.argv[4]

        ChangeConfigLine(config_dir, config_file, line_name, new_value)

    else:
        print('''
        Args:
            [1] - Script name or key (from dict at top of %s)
            [2] - line name to change
            [3] - new value
        ''' % sys.argv[0])