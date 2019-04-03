import numpy as np
import sys
import matplotlib as mpl
mpl.use('agg')
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import matplotlib.cm as cm


X_TICKS = [0.0, 17.5, 35, 52.5, 70, 87.5, 105.0]
Y_TICKS = [0.0, 13.6, 27.2, 40.8, 54.4, 68.0]

GRID_TICKS = { # [bottom left corner, top right corner] 
    0 : [ [0, 4] , [1, 5] ],
    1 : [ [1, 4] , [2, 5] ],
    2 : [ [2, 4] , [3, 5] ],
    3 : [ [3, 4] , [4, 5] ],
    4 : [ [4, 4] , [5, 5] ],
    5 : [ [5, 4] , [6, 5] ],
    6 : [ [0, 1] , [1, 4] ],
    7 : [ [1, 3] , [2, 4] ],
    8 : [ [2, 3] , [3, 4] ],
    9 : [ [3, 3] , [4, 4] ],
    10 : [ [4, 3] , [5, 4] ],
    11 : [ [5, 1] , [6, 4] ],
    12 : [ [1, 2] , [2, 3] ],
    13 : [ [2, 2] , [3, 3] ],
    14 : [ [3, 2] , [4, 3] ],
    15 : [ [4, 2] , [5, 3] ],
    16 : [ [1, 1] , [2, 2] ],
    17 : [ [2, 1] , [3, 2] ],
    18 : [ [3, 1] , [4, 2] ],
    19 : [ [4, 1] , [5, 2] ],
    20 : [ [0, 0] , [1, 1] ],
    21 : [ [1, 0] , [2, 1] ],
    22 : [ [2, 0] , [3, 1] ],
    23 : [ [3, 0] , [4, 1] ],
    24 : [ [4, 0] , [5, 1] ],
    25 : [ [5, 0] , [6, 1] ]
}

def load_csv_data(filename : str):
    with open(filename, 'r') as raw_data:
        data = np.loadtxt(raw_data)

    return(data)


def save_cpp_data(filename : str, data):
    split_filename = filename.split('.')[:-1]

    modded_filename = ''

    for spfn in split_filename:
        modded_filename += spfn

    modded_filename += '_array.cpp'

    data_shape = data.shape
    rows = data_shape[0]
    columns = data_shape[1]

    sum = 0.0

    with open(modded_filename, 'w') as array_file:
        array_file.write('double myArray[%i][%i] = { ' % (rows, columns))

        for i in range(rows):
            if i != 0:
                array_file.write(',') # separating comma

            array_file.write('\n\t{') # open row bracket

            for j in range(columns):
                if j != 0:
                    array_file.write(',')
                else:
                    sum += data[i][3]
                
                array_file.write(' %.3e ' % data[i][j])

            array_file.write('}')
        
        array_file.write('\n};')
    print(sum)

def generate_patch(idx, colour):
    grid_ticks_entry = GRID_TICKS[idx]
    blc = grid_ticks_entry[0] ; trc = grid_ticks_entry[1]
    blc_coords = [X_TICKS[blc[0]], Y_TICKS[blc[1]]] ; trc_coords = [X_TICKS[trc[0]], Y_TICKS[trc[1]]]
    x_length = trc_coords[0] - blc_coords[0]
    y_length = trc_coords[1] - blc_coords[1]

    center = [0.5 * (blc_coords[0] + trc_coords[0]) , 0.5 * (blc_coords[1] + trc_coords[1])]

    rect = patches.Rectangle((blc_coords[0], blc_coords[1]), x_length, y_length, linewidth=1.0, edgecolor='k', facecolor=colour)
    return(rect, center)

def plot_heatmap(eigenvalue, weights, num):
    
    mod_eigenvalue = abs(eigenvalue)

    fig,ax = plt.subplots(1)

    ax.set_xlim(0, 105.0)
    ax.set_ylim(0, 68.0)

    norm = mpl.colors.Normalize(vmin=0.0, vmax=1.0, clip=True)
    mapper = cm.ScalarMappable(norm=norm, cmap=cm.coolwarm)
    mapper_complex = cm.ScalarMappable(norm=norm, cmap=cm.Greens)

    for i, w in enumerate(weights):
        if type(w) == np.complex128:
            if w != np.conj(w):
                colour = mapper_complex.to_rgba(abs(w))
            else:
                colour = mapper.to_rgba(abs(w))
        else:
            colour = mapper.to_rgba(w)

        rect, center = generate_patch(i, colour)
        ax.add_patch(rect)
        ax.text(center[0] - 4.5, center[1] - 1.0, '%.3f' % abs(w))

    if type(eigenvalue) == np.complex128:
        if eigenvalue != np.conj(eigenvalue):
            # complex
            #fig.colorbar(ax)
            ax.set_title('Eigenvalue: %.3e + %.3ei (mod: %.3e)' % (eigenvalue.real, eigenvalue.imag, mod_eigenvalue))
        else:
            # real
            #fig.colorbar(ax)
            ax.set_title('Eigenvalue: %.3e' % (eigenvalue.real))
    else:
        ax.set_title('Eigenvalue: %.3e' % (eigenvalue.real))

    


    fig.savefig('plot%i.png' % num)

    plt.close()


def main(filename: str):
    raw_data = load_csv_data(filename)
    data = np.swapaxes(raw_data, 0, 1)
    normed_data = data/data.sum(axis=0)

    W, V = np.linalg.eig(normed_data)
    
    for i, w in enumerate(W):
        print('%02d' % i, 'w: ', w)
        plot_heatmap(w, V[:,i], i)
        #print('vector:')
        #print(V[:,i])
    save_cpp_data(filename, normed_data)

if __name__ == '__main__':
    if len(sys.argv) == 1:
        print('enter full csv path to convert to array')
        exit()
    elif len(sys.argv) == 2:
        filename = sys.argv[1]
        main(filename)
    else:
        print('enter full csv path only')
        exit()
    