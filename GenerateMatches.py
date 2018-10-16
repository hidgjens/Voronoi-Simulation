import subprocess as s
import queue
import threading
from os import makedirs
from os.path import exists
import sys

def main(run_type, samples, frames, filename, legacy, processes):
    # check output folder exists
    if not exists('data_files/csvs'):
        makedirs('data_files/csvs')

    if legacy == 'yes':
        if not exists('data_files/legacy_csvs'):
            makedirs('data_files/legacy_csvs')

    # work out a list of samples and start num for each thread
    std_smps = samples // processes # samples per thread
    left_over = samples % processes # extras,
    # can either just add to first bin or add one extra to each thread (more efficient)
    # create list of batches
    batches = [std_smps for i in range(processes)]
    # add extras
    for i in range(left_over):
        batches[i] += 1
    # for each batch need to know what the first sample number is, for filemanagement
    ctr = 0
    batch_data = []
    for batch in batches:
        batch_data.append([batch, ctr])
        ctr += batch

    # create worker function
    def worker(samples_, start_num):
        print(start_num)
        if run_type == 'UnitSquare':
            cmdlist = ['bin/sim', filename, str(samples_), str(frames), str(start_num), legacy, 'UnitPolygon', '4']
        else:
            cmdlist = ['bin/sim', filename, str(samples_), str(frames), str(start_num), legacy, run_type]
        s.call(cmdlist)

    # start threads
    threads = [threading.Thread(target=worker, args=(*data,)) for _i, data in zip(range(processes), batch_data)]
    for thread in threads:
        thread.start()


if __name__ == '__main__':
    # process sys args
    if len(sys.argv) == 7:
        run_type = sys.argv[1]
        samples = int(sys.argv[2])
        frames = int(sys.argv[3])
        filename = sys.argv[4]
        legacy = sys.argv[5]
        processes = int(sys.argv[6])
    elif len(sys.argv) == 2:
        run_type = sys.argv[1]
        samples = 400
        frames = 15000
        filename = sys.argv[1]
        legacy = sys.argv[5]
        processes = 8
    else:
        print('''
    %s - Runs the simulation multiple times
        args:
        [1] - Run Type (RandomWalk/1HRandomWalk/1HExchange/UnitSquare)
        [2] - Samples [default: 400]
        [3] - Frames per sample [default: 15000]
        [4] - Filename [default: (Run Type)]
        [5] - Legacy file (yes/no) [default: (no)]
        [6] - Processes [default: 8]
        ''' % sys.argv[0])
        exit()
    # args complete, run main function
    main(run_type, samples, frames, filename, legacy, processes)
