PITCH_X = 105.0
PITCH_Y = 68.0

X_W_MIN = 0.5 # minimum value of omega for x scale
X_W_MAX = 1.5 # maximum " " " " " "

Y_W_MIN = 0.5 # minimum " " " " y scale
Y_W_MAX = 1.5 # maximum " " " " " "

# used in linear map
X_W_BAR = 0.5 * (X_W_MIN + X_W_MAX)
X_W_DELTA = (X_W_MAX - X_W_MIN)

# used in quadratic map
A = 4 * (Y_W_MAX - Y_W_MIN) / (PITCH_Y ** 2)
Ax = 4 * (X_W_MAX - X_W_MIN) / (PITCH_X ** 2)


SCALE = 0.75

# imports
import numpy as np
import matplotlib as mpl 
mpl.use('agg')
import seaborn as sns

import matplotlib.pyplot as plt


def pitch_model_linear(x : float, y : float) -> float:
    # y component is just 1
    # just need x component

    x_cmpt : float = X_W_BAR + X_W_DELTA * (x / PITCH_X)

    return(x_cmpt)

def pitch_model_linquad(x : float, y : float) -> float:
    x_cmpt : float = X_W_BAR + X_W_DELTA * (x / PITCH_X)
    y_cmpt : float = - A * ( y - PITCH_Y / 2.0) * ( y + PITCH_Y / 2.0) + Y_W_MIN

    return(x_cmpt * y_cmpt)

def pitch_model_quadquad(x : float, y : float) -> float:
    x_cmpt : float = x**2 * (X_W_DELTA) + X_W_MIN
    y_cmpt : float = - A * ( y - PITCH_Y / 2.0) * ( y + PITCH_Y / 2.0) + Y_W_MIN

    return(x_cmpt * y_cmpt)

def pitch_model_uniform(x : float, y : float) -> float:
    return(1.0)

def pitch_model_delta(x : float, y: float) -> float:
    x_cmpt : float = X_W_BAR + X_W_DELTA * (x / PITCH_X)
    
    if y == 0.0:
        return(x_cmpt)
    else:
        return(0.0)


def convert_x_idx_to_coord(i : int) -> float:
    return( ((i + 0.5) - 0.5 * int(PITCH_X)) * PITCH_X / int(PITCH_X) )

def convert_y_idx_to_coord(j : int) -> float:
    return( ((j + 0.5) - 0.5 * int(PITCH_Y)) * PITCH_Y / int(PITCH_Y) )

def create_heatmap_info(model_function):

    x_samples : int = int(PITCH_X)
    y_samples : int = int(PITCH_Y)

    sum_w_rsquared : float = 0.0 ; sum_w_xsquared_above : float = 0.0 ; sum_w_xsquared_below : float = 0.0 ; sum_w_ysquared_above : float = 0.0 ; sum_w_ysquared_below : float = 0.0 

    v_sum_wx : float = 0.0
    v_sum_wy : float = 0.0 # mean coord vector sum

    Z : float = 0.0 ; Zx_above : float = 0.0 ; Zx_below : float = 0.0 ; Zy_above : float = 0.0 ; Zy_below : float = 0.0 # omega sum (norm)

    temp_x : float ; temp_y : float ; temp_w : float

    heatmap = np.zeros((y_samples, x_samples))

    for i in range(x_samples):
        for j in range(y_samples):
            temp_x = convert_x_idx_to_coord(i)
            temp_y = convert_y_idx_to_coord(j)
            # print("(%f, %f)" % (temp_x, temp_y))
            temp_w = model_function(temp_x, temp_y)

            heatmap[j,i] = temp_w
            Z += temp_w
            v_sum_wx += temp_x * temp_w
            v_sum_wy += temp_y * temp_w
            sum_w_rsquared += (temp_x ** 2 + temp_y ** 2) * temp_w

    # norm
    # heatmap = np.divide(heatmap, Z)
    sum_w_rsquared /= Z
    v_sum_wx /= Z
    v_sum_wy /= Z

    WAC = [v_sum_wx, v_sum_wy]

    sigma : float = np.sqrt(sum_w_rsquared - (v_sum_wx ** 2 + v_sum_wy ** 2))

    for i in range(x_samples):
        temp_x = convert_x_idx_to_coord(i)
        temp_y = v_sum_wy
        # print("(%f, %f)" % (temp_x, temp_y))
        temp_w = model_function(temp_x, temp_y)

        if (temp_x > v_sum_wx):
            sum_w_xsquared_above += (temp_x ** 2 + temp_y ** 2) * temp_w
            Zx_above += temp_w
        else:
            sum_w_xsquared_below += (temp_x ** 2 + temp_y ** 2) * temp_w
            Zx_below += temp_w
    
    for j in range(y_samples):
        temp_x = v_sum_wx
        temp_y = convert_y_idx_to_coord(j)
        # print("(%f, %f)" % (temp_x, temp_y))
        temp_w = model_function(temp_x, temp_y)

        if (temp_y > v_sum_wy):
            sum_w_ysquared_above += (temp_x ** 2 + temp_y ** 2) * temp_w
            Zy_above += temp_w
        else:
            sum_w_ysquared_below += (temp_x ** 2 + temp_y ** 2) * temp_w
            Zy_below += temp_w

    if Zy_above == 0: Zy_above = 1
    if Zy_below == 0: Zy_below = 1
    if Zx_above == 0: Zx_above = 1
    if Zx_below == 0: Zx_below = 1

    sum_w_xsquared_above /= Zx_above ; sum_w_xsquared_below /= Zx_below
    sum_w_ysquared_above /= Zy_above ; sum_w_ysquared_below /= Zy_below

    x_sigma_above : float = np.sqrt(sum_w_xsquared_above - (v_sum_wx ** 2 + v_sum_wy ** 2))
    x_sigma_below : float = np.sqrt(sum_w_xsquared_below - (v_sum_wx ** 2 + v_sum_wy ** 2))

    y_sigma_above : float = np.sqrt(sum_w_ysquared_above - (v_sum_wx ** 2 + v_sum_wy ** 2))
    y_sigma_below : float = np.sqrt(sum_w_ysquared_below - (v_sum_wx ** 2 + v_sum_wy ** 2))
    

    return(heatmap, WAC, sigma, [x_sigma_above, x_sigma_below], [y_sigma_above, y_sigma_below])

def plot_heatmap(pitch_model_function, fn):
    hm, WAC, sigma, x_sigmas, y_sigmas = create_heatmap_info(pitch_model_function)
    print("\n", fn)
    print("WAC: ", WAC)
    print("sigma_x: %f (+%f/-%f)" % ( sigma, *x_sigmas) )
    print("sigma_y: %f (+%f/-%f)" % ( sigma, *y_sigmas) )


    plt.figure()
    ax = plt.gca()
    im = ax.imshow(hm, cmap = 'Reds', extent=[-0.5 * PITCH_X, 0.5 * PITCH_X, -0.5 * PITCH_Y, 0.5 * PITCH_Y])
    ax.autoscale(False)

    # lines
    x_plus = WAC[0] + SCALE * x_sigmas[0]
    x_minus = WAC[0] - SCALE * x_sigmas[1]
    y_plus = WAC[1] + SCALE * y_sigmas[0]
    y_minus = WAC[1] - SCALE * y_sigmas[1]

    plt.plot([x_plus, x_plus], [0.5 * PITCH_Y, -0.5 * PITCH_Y], 'g-')
    plt.plot([x_minus, x_minus], [0.5 * PITCH_Y, -0.5 * PITCH_Y], 'g-')
    plt.plot([0.5 * PITCH_X, -0.5 * PITCH_X], [y_plus, y_plus], 'g-')
    plt.plot([0.5 * PITCH_X, -0.5 * PITCH_X], [y_minus, y_minus], 'g-')


    # formatting
    plt.xticks([])
    plt.yticks([])
    
    # colorbar formatting
    plt.colorbar(im, fraction = 0.03, pad = 0.04)#, ticks = [X_W_MIN, X_W_MIN + .25 * (X_W_DELTA), X_W_MIN + .5 * (X_W_DELTA), X_W_MIN + .75 * (X_W_DELTA), X_W_MAX])
    ax.scatter(WAC[0], WAC[1], c='g')
    ax.set_aspect("equal")
    
    plt.title(fn)
    plt.savefig('hm-%s.png' % fn, bbox_inches='tight')

    plt.clf()

def loadHeatmap():
    hm = np.loadtxt('heatmap.csv')
    print(hm.shape)
    fn = 'sim'

    x_samples : int = int(PITCH_X)
    y_samples : int = int(PITCH_Y)

    sum_w_rsquared : float = 0.0 ; sum_w_xsquared_above : float = 0.0 ; sum_w_xsquared_below : float = 0.0 ; sum_w_ysquared_above : float = 0.0 ; sum_w_ysquared_below : float = 0.0 

    v_sum_wx : float = 0.0
    v_sum_wy : float = 0.0 # mean coord vector sum

    Z : float = 0.0 ; Zx_above : float = 0.0 ; Zx_below : float = 0.0 ; Zy_above : float = 0.0 ; Zy_below : float = 0.0 # omega sum (norm)

    temp_x : float ; temp_y : float ; temp_w : float

    heatmap = hm

    for i in range(x_samples):
        for j in range(y_samples):
            temp_x = convert_x_idx_to_coord(i)
            temp_y = convert_y_idx_to_coord(j)
            # print("(%f, %f)" % (temp_x, temp_y))
            temp_w = heatmap[j,i]

            Z += temp_w
            v_sum_wx += temp_x * temp_w
            v_sum_wy += temp_y * temp_w
            sum_w_rsquared += (temp_x ** 2 + temp_y ** 2) * temp_w

    # norm
    # heatmap = np.divide(heatmap, Z)
    sum_w_rsquared /= Z
    v_sum_wx /= Z
    v_sum_wy /= Z

    WAC = [v_sum_wx, v_sum_wy]

    sigma : float = np.sqrt(sum_w_rsquared - (v_sum_wx ** 2 + v_sum_wy ** 2))

    for i in range(x_samples):
        temp_x = convert_x_idx_to_coord(i)
        temp_y = v_sum_wy

        temp_w = heatmap[int(int(v_sum_wy) - y_samples/2), i]

        if (temp_x > v_sum_wx):
            sum_w_xsquared_above += (temp_x ** 2 + temp_y ** 2) * temp_w
            Zx_above += temp_w
        else:
            sum_w_xsquared_below += (temp_x ** 2 + temp_y ** 2) * temp_w
            Zx_below += temp_w
    
    for j in range(y_samples):
        temp_x = v_sum_wx
        temp_y = convert_y_idx_to_coord(j)

        temp_w = heatmap[j, int(int(v_sum_wx) - x_samples/2)]

        if (temp_y > v_sum_wy):
            sum_w_ysquared_above += (temp_x ** 2 + temp_y ** 2) * temp_w
            Zy_above += temp_w
        else:
            sum_w_ysquared_below += (temp_x ** 2 + temp_y ** 2) * temp_w
            Zy_below += temp_w

    if Zy_above == 0: Zy_above = 1
    if Zy_below == 0: Zy_below = 1
    if Zx_above == 0: Zx_above = 1
    if Zx_below == 0: Zx_below = 1

    sum_w_xsquared_above /= Zx_above ; sum_w_xsquared_below /= Zx_below
    sum_w_ysquared_above /= Zy_above ; sum_w_ysquared_below /= Zy_below

    x_sigma_above : float = np.sqrt(sum_w_xsquared_above - (v_sum_wx ** 2 + v_sum_wy ** 2))
    x_sigma_below : float = np.sqrt(sum_w_xsquared_below - (v_sum_wx ** 2 + v_sum_wy ** 2))

    y_sigma_above : float = np.sqrt(sum_w_ysquared_above - (v_sum_wx ** 2 + v_sum_wy ** 2))
    y_sigma_below : float = np.sqrt(sum_w_ysquared_below - (v_sum_wx ** 2 + v_sum_wy ** 2))
    
    x_sigmas = [x_sigma_above, x_sigma_below]
    y_sigmas = [y_sigma_above, y_sigma_below]

    print("\n", fn)
    print("WAC: ", WAC)
    print("sigma_x: %f (+%f/-%f)" % ( sigma, *x_sigmas) )
    print("sigma_y: %f (+%f/-%f)" % ( sigma, *y_sigmas) )


    plt.figure()
    ax = plt.gca()
    im = ax.imshow(hm, cmap = 'Reds', extent=[-0.5 * PITCH_X, 0.5 * PITCH_X, -0.5 * PITCH_Y, 0.5 * PITCH_Y])
    ax.autoscale(False)

    # lines
    x_plus = WAC[0] + SCALE * x_sigmas[0]
    x_minus = WAC[0] - SCALE * x_sigmas[1]
    y_plus = WAC[1] + SCALE * y_sigmas[0]
    y_minus = WAC[1] - SCALE * y_sigmas[1]

    plt.plot([x_plus, x_plus], [0.5 * PITCH_Y, -0.5 * PITCH_Y], 'g-')
    plt.plot([x_minus, x_minus], [0.5 * PITCH_Y, -0.5 * PITCH_Y], 'g-')
    plt.plot([0.5 * PITCH_X, -0.5 * PITCH_X], [y_plus, y_plus], 'g-')
    plt.plot([0.5 * PITCH_X, -0.5 * PITCH_X], [y_minus, y_minus], 'g-')


    # formatting
    plt.xticks([])
    plt.yticks([])
    
    # colorbar formatting
    plt.colorbar(im, fraction = 0.03, pad = 0.04)#, ticks = [X_W_MIN, X_W_MIN + .25 * (X_W_DELTA), X_W_MIN + .5 * (X_W_DELTA), X_W_MIN + .75 * (X_W_DELTA), X_W_MAX])
    ax.scatter(WAC[0], WAC[1], c='g')
    ax.set_aspect("equal")
    
    plt.title(fn)
    plt.savefig('hm-%s.png' % fn, bbox_inches='tight')

    plt.clf()
    


def main():
    loadHeatmap()
    plot_heatmap(pitch_model_linear, "lin")
    plot_heatmap(pitch_model_linquad, "linquad")
    plot_heatmap(pitch_model_quadquad, "quadquad")
    plot_heatmap(pitch_model_delta, 'delta')
    plot_heatmap(pitch_model_uniform, "uni")

if __name__ == '__main__':
    main()

