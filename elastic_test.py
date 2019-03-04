import numpy as np
import matplotlib as mpl
mpl.use('agg')
from matplotlib import pyplot as plt
from os import makedirs
from os.path import exists
import subprocess as s
import copy

R = 15.0 # m 
step_size = 2.5 # m
PITCH_DIMS = np.asarray([105.0, 68.0])

# F = kr, require F = 2.5 when r = R

k = step_size / R
print('Elasticity constant %f' % k)

def restoring_displacement(r):
    return(k*r)

def random_walk():
    rand_dir = np.multiply(np.random.rand(), 2*np.pi) # [0,1]
    #translated_rand_dr = np.subtract(rand_dr, 0.5) # [-0.5, 0.5]
    dr = np.asarray([step_size * np.cos(rand_dir), step_size * np.sin(rand_dir)])
    return(dr)

class Player:
    position = np.asarray([0.0,0.0])

    def __init__(self):
        print('Player')

    def changePosition(self, dr):
        self.position = np.add(self.position,dr)

    def printPosition(self):
        print("Position: ", self.position)

    def getR(self):
        R = np.sqrt(self.position[0] ** 2 + self.position[1] ** 2)
        return(R)

    def getUnitVector(self):
        R = self.getR()
        if R == 0:
            return(np.asarray([0,0]))
        else:
            unit_x = self.position[0] / R
            unit_y = self.position[1] / R
            return(np.asarray([unit_x, unit_y]))


def main():
    player = Player()
    old_R = player.getR()

    positions = []
    # for plots
    Rs = [old_R] 
    Is = [0]

    if not exists('elastic'):
        makedirs('elastic')


    fig, ax = plt.subplots()

    steps = 100000

    for i in range(steps):
        #print('Restore ', restoring_displacement(old_R), "\n", np.multiply(np.divide(player.position, player.getR()), -1.0 * restoring_displacement(old_R)))
        rw = random_walk()
        if not old_R == 0:
            restore = np.multiply(np.multiply(np.divide(player.position, player.getR()),  restoring_displacement(old_R)), -1.0)
            print(restore, "not")
        else:
            restore = np.asarray([0.0,0.0])
            print(restore, "is")

        if not old_R == 0.0:
            dr = np.add(rw, restore)
        else:
            dr = rw

        old_pos = copy.deepcopy(player.position)
        print(player.position)
        print(old_pos)
        player.changePosition(dr)
        print(player.position)
        print(old_pos)

        #print(old_pos.all() == player.position.all())

        player.printPosition()
        positions.append(player.position)
    

        new_R = player.getR()
        Rs.append(new_R)
        Is.append(i + 1)
        print('new_R: %f' % new_R)
        print('dR: %f' % (new_R - old_R))
        old_R = new_R
        
        
        if not restore.all() == np.asarray([0.0, 0.0]).all():
            ax.arrow(old_pos[0], old_pos[1], restore[0], restore[1], color = 'r', length_includes_head=False, width = 0.1)

        ax.arrow(old_pos[0], old_pos[1], rw[0], rw[1], color = 'b', length_includes_head=False, width = 0.1)
        ax.arrow(old_pos[0], old_pos[1], dr[0], dr[1], color = 'g', length_includes_head=False, width = 0.1)

        ax.scatter(player.position[0], player.position[1], color = 'k')

        circle1 = plt.Circle((0, 0), R, color='r', fill=False)
        ax.add_artist(circle1)
        ax.set_xlim(-R, R)
        ax.set_ylim(-R, R)
        ax.set_aspect('equal')
        fig.savefig('elastic/%06d.png' % (i))
        ax.clear()
        ax.set_aspect('auto')


    plt.plot(Is, Rs)
    plt.xlim(0, steps)
    plt.ylim(0, R)
    plt.xlabel('Steps')
    plt.ylabel('Distance from post')
    plt.savefig('elastic_tether.png')

    s.call(['ffmpeg','-framerate','5','-i','elastic/%06d.png','elastic.mp4','-vcodec','mpeg4','-y'])
    

if __name__ == '__main__':
    main()






