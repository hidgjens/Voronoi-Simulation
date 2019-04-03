#ifndef BALL_H
#define BALL_H

#define NUM_OF_TILES 26

#include <memory>
#include "Cart.h"
#include "Pitch.h"
#include "Tile.h"

class Ball {

private:
    Cart pos;
    Pitch pitch_data;

    double player_step_size = 2.5; // metres
    double max_multiplier = 0.75; // multiples of player_step_size for maximum random walk step
    double min_multiplier = 4.0; // multiples of player_step_size for minumum random walk step

    int tile_index; // tile coordinates
    int actual_num_of_tiles;

    std::unique_ptr<Tile[]> tiles;

    const double stochastic_matrix[NUM_OF_TILES][NUM_OF_TILES] = {
        { 2.613e-01 , 7.015e-02 , 9.141e-03 , 6.897e-04 , 0.000e+00 , 4.594e-04 , 5.352e-02 , 1.854e-02 , 3.262e-03 , 3.647e-04 , 0.000e+00 , 1.095e-04 , 6.735e-03 , 1.565e-03 , 4.901e-05 , 0.000e+00 , 1.515e-03 , 4.754e-04 , 0.000e+00 , 0.000e+00 , 4.398e-03 , 3.862e-04 , 9.556e-05 , 0.000e+00 , 0.000e+00 , 0.000e+00 },
        { 1.855e-01 , 3.001e-01 , 1.110e-01 , 1.685e-02 , 1.478e-03 , 1.225e-03 , 5.140e-02 , 1.125e-01 , 2.687e-02 , 5.774e-03 , 9.422e-04 , 6.572e-04 , 4.100e-02 , 7.751e-03 , 2.794e-03 , 5.393e-04 , 8.533e-03 , 1.215e-03 , 5.572e-04 , 1.236e-04 , 1.675e-03 , 7.724e-04 , 9.556e-05 , 8.999e-05 , 0.000e+00 , 0.000e+00 },
        { 8.323e-02 , 1.705e-01 , 3.018e-01 , 1.337e-01 , 1.870e-02 , 4.135e-03 , 2.439e-02 , 1.151e-01 , 1.434e-01 , 3.513e-02 , 9.068e-03 , 3.615e-03 , 4.799e-02 , 6.274e-02 , 7.155e-03 , 2.966e-03 , 5.981e-03 , 1.030e-02 , 1.548e-03 , 2.472e-04 , 6.283e-04 , 6.180e-04 , 1.147e-03 , 4.500e-04 , 6.518e-05 , 3.174e-04 },
        { 1.177e-02 , 4.324e-02 , 1.607e-01 , 3.533e-01 , 1.532e-01 , 3.292e-02 , 1.684e-02 , 2.397e-02 , 1.204e-01 , 1.765e-01 , 3.792e-02 , 9.530e-03 , 1.357e-02 , 4.760e-02 , 6.102e-02 , 7.820e-03 , 3.748e-03 , 8.926e-03 , 1.071e-02 , 9.886e-04 , 4.188e-04 , 4.635e-04 , 1.959e-03 , 2.070e-03 , 3.259e-04 , 0.000e+00 },
        { 1.239e-03 , 6.308e-03 , 3.120e-02 , 1.400e-01 , 3.934e-01 , 1.982e-01 , 1.888e-03 , 5.121e-03 , 2.050e-02 , 1.106e-01 , 1.789e-01 , 2.585e-02 , 4.110e-03 , 1.387e-02 , 4.582e-02 , 4.396e-02 , 6.380e-04 , 7.289e-03 , 1.226e-02 , 6.179e-03 , 0.000e+00 , 0.000e+00 , 1.242e-03 , 2.205e-03 , 8.473e-04 , 3.174e-04 },
        { 8.261e-04 , 2.280e-04 , 2.519e-03 , 1.397e-02 , 9.317e-02 , 3.179e-01 , 2.997e-05 , 2.328e-04 , 6.731e-04 , 9.299e-03 , 6.878e-02 , 6.441e-02 , 4.952e-05 , 5.459e-04 , 5.489e-03 , 1.546e-02 , 0.000e+00 , 5.282e-04 , 3.653e-03 , 4.572e-03 , 0.000e+00 , 0.000e+00 , 4.778e-05 , 2.250e-04 , 1.043e-03 , 6.665e-03 },
        { 3.265e-01 , 1.301e-01 , 2.458e-02 , 9.483e-04 , 6.425e-05 , 1.531e-04 , 3.516e-01 , 1.580e-01 , 2.335e-02 , 1.580e-03 , 4.711e-04 , 9.859e-04 , 1.476e-01 , 2.118e-02 , 1.960e-03 , 1.798e-04 , 1.534e-01 , 2.308e-02 , 1.176e-03 , 2.472e-04 , 3.194e-01 , 1.220e-01 , 2.389e-02 , 1.035e-03 , 2.607e-04 , 3.174e-04 },
        { 5.452e-02 , 1.191e-01 , 5.336e-02 , 7.673e-03 , 1.157e-03 , 0.000e+00 , 6.365e-02 , 1.426e-01 , 4.323e-02 , 5.835e-03 , 1.295e-03 , 2.191e-04 , 8.894e-02 , 1.699e-02 , 3.333e-03 , 1.258e-03 , 5.184e-02 , 7.870e-03 , 7.430e-04 , 2.472e-04 , 2.723e-03 , 6.180e-03 , 1.816e-03 , 2.250e-04 , 6.518e-05 , 1.587e-04 },
        { 1.983e-02 , 5.092e-02 , 1.409e-01 , 6.203e-02 , 8.032e-03 , 2.757e-03 , 1.537e-02 , 7.976e-02 , 1.592e-01 , 6.364e-02 , 9.657e-03 , 4.491e-03 , 5.210e-02 , 1.092e-01 , 2.509e-02 , 7.371e-03 , 1.962e-02 , 4.960e-02 , 8.978e-03 , 4.943e-04 , 4.188e-04 , 1.699e-03 , 5.160e-03 , 2.205e-03 , 6.518e-05 , 0.000e+00 },
        { 1.033e-02 , 1.923e-02 , 4.137e-02 , 1.358e-01 , 5.764e-02 , 9.801e-03 , 1.882e-02 , 1.854e-02 , 8.170e-02 , 1.737e-01 , 6.136e-02 , 8.435e-03 , 1.149e-02 , 4.367e-02 , 1.084e-01 , 2.661e-02 , 2.791e-03 , 7.711e-03 , 2.891e-02 , 5.190e-03 , 2.094e-04 , 1.468e-03 , 2.007e-03 , 3.195e-03 , 1.108e-03 , 0.000e+00 },
        { 2.685e-03 , 3.648e-03 , 1.082e-02 , 3.470e-02 , 1.207e-01 , 5.421e-02 , 2.997e-03 , 3.491e-03 , 1.196e-02 , 6.005e-02 , 2.016e-01 , 4.130e-02 , 5.695e-03 , 9.535e-03 , 3.686e-02 , 9.393e-02 , 1.755e-03 , 5.282e-03 , 8.730e-03 , 1.780e-02 , 0.000e+00 , 2.317e-04 , 2.485e-03 , 2.565e-03 , 2.151e-03 , 3.174e-03 },
        { 2.065e-04 , 6.080e-04 , 4.244e-03 , 1.789e-02 , 1.012e-01 , 3.519e-01 , 2.997e-04 , 4.655e-04 , 3.779e-03 , 2.656e-02 , 1.828e-01 , 5.831e-01 , 6.438e-04 , 4.622e-03 , 3.019e-02 , 1.953e-01 , 8.773e-04 , 4.965e-03 , 2.941e-02 , 1.789e-01 , 1.047e-03 , 3.090e-04 , 4.921e-03 , 2.245e-02 , 1.185e-01 , 3.546e-01 },
        { 1.714e-02 , 5.388e-02 , 2.808e-02 , 3.362e-03 , 4.498e-04 , 1.531e-04 , 1.027e-01 , 1.567e-01 , 4.551e-02 , 4.437e-03 , 7.066e-04 , 9.859e-04 , 1.901e-01 , 5.255e-02 , 1.015e-02 , 2.517e-03 , 1.416e-01 , 4.220e-02 , 5.139e-03 , 4.943e-04 , 2.199e-02 , 5.793e-02 , 2.566e-02 , 2.835e-03 , 2.607e-04 , 1.587e-04 },
        { 9.500e-03 , 1.345e-02 , 5.009e-02 , 2.366e-02 , 2.634e-03 , 4.594e-04 , 3.542e-02 , 6.820e-02 , 1.700e-01 , 5.531e-02 , 6.477e-03 , 1.260e-02 , 1.108e-01 , 2.095e-01 , 7.675e-02 , 1.609e-02 , 5.965e-02 , 1.737e-01 , 5.436e-02 , 6.550e-03 , 7.749e-03 , 1.344e-02 , 5.189e-02 , 2.524e-02 , 2.216e-03 , 2.380e-03 },
        { 4.131e-03 , 4.408e-03 , 1.026e-02 , 3.078e-02 , 1.600e-02 , 2.603e-03 , 2.296e-02 , 1.094e-02 , 4.302e-02 , 1.526e-01 , 4.970e-02 , 3.505e-02 , 1.971e-02 , 8.355e-02 , 2.205e-01 , 7.982e-02 , 1.204e-02 , 4.859e-02 , 1.550e-01 , 5.413e-02 , 5.026e-03 , 8.111e-03 , 1.166e-02 , 3.271e-02 , 1.506e-02 , 2.698e-03 },
        { 6.196e-04 , 2.052e-03 , 5.690e-03 , 1.043e-02 , 2.609e-02 , 1.394e-02 , 3.956e-03 , 1.784e-03 , 7.973e-03 , 4.145e-02 , 1.511e-01 , 7.197e-02 , 3.714e-03 , 1.296e-02 , 7.748e-02 , 3.030e-01 , 2.632e-03 , 1.046e-02 , 4.365e-02 , 1.514e-01 , 6.283e-04 , 1.854e-03 , 7.884e-03 , 1.237e-02 , 2.366e-02 , 1.507e-02 },
        { 5.370e-03 , 6.992e-03 , 1.866e-03 , 3.017e-04 , 0.000e+00 , 1.531e-04 , 5.982e-02 , 4.469e-02 , 6.627e-03 , 6.686e-04 , 3.533e-04 , 6.572e-04 , 8.434e-02 , 1.881e-02 , 4.264e-03 , 1.438e-03 , 1.423e-01 , 4.104e-02 , 5.387e-03 , 1.112e-03 , 4.545e-02 , 1.188e-01 , 5.638e-02 , 7.379e-03 , 5.866e-04 , 4.760e-04 },
        { 0.000e+00 , 2.052e-03 , 6.156e-03 , 1.983e-03 , 6.425e-05 , 6.126e-04 , 1.600e-02 , 1.691e-02 , 4.742e-02 , 8.935e-03 , 8.244e-04 , 4.601e-03 , 4.710e-02 , 1.065e-01 , 2.284e-02 , 7.101e-03 , 8.238e-02 , 1.527e-01 , 6.749e-02 , 9.639e-03 , 2.995e-02 , 5.786e-02 , 1.405e-01 , 6.605e-02 , 7.691e-03 , 2.539e-03 },
        { 4.131e-04 , 6.840e-04 , 1.306e-03 , 3.535e-03 , 5.783e-04 , 3.063e-04 , 1.693e-02 , 3.103e-03 , 8.284e-03 , 2.899e-02 , 4.829e-03 , 7.230e-03 , 1.159e-02 , 4.101e-02 , 1.055e-01 , 2.382e-02 , 2.089e-02 , 8.118e-02 , 1.735e-01 , 6.636e-02 , 1.068e-02 , 1.970e-02 , 4.534e-02 , 1.342e-01 , 5.410e-02 , 9.045e-03 },
        { 4.131e-04 , 7.600e-05 , 1.073e-03 , 2.328e-03 , 2.506e-03 , 1.378e-03 , 3.536e-03 , 1.009e-03 , 3.624e-03 , 8.995e-03 , 1.790e-02 , 3.768e-02 , 3.467e-03 , 7.387e-03 , 3.627e-02 , 9.285e-02 , 3.350e-03 , 1.516e-02 , 6.049e-02 , 1.894e-01 , 1.257e-03 , 3.939e-03 , 1.481e-02 , 3.703e-02 , 1.165e-01 , 5.173e-02 },
        { 2.891e-03 , 6.840e-04 , 0.000e+00 , 0.000e+00 , 0.000e+00 , 0.000e+00 , 5.175e-02 , 1.397e-03 , 2.589e-04 , 0.000e+00 , 0.000e+00 , 1.095e-04 , 7.230e-03 , 1.310e-03 , 9.802e-05 , 0.000e+00 , 1.970e-02 , 1.954e-03 , 3.096e-04 , 0.000e+00 , 2.568e-01 , 6.844e-02 , 8.027e-03 , 7.649e-04 , 6.518e-05 , 0.000e+00 },
        { 1.239e-03 , 3.800e-04 , 3.265e-04 , 1.293e-04 , 0.000e+00 , 1.531e-04 , 4.969e-02 , 6.983e-03 , 1.294e-03 , 5.470e-04 , 0.000e+00 , 7.668e-04 , 3.991e-02 , 8.370e-03 , 2.647e-03 , 8.989e-04 , 1.223e-01 , 2.593e-02 , 6.501e-03 , 1.359e-03 , 1.858e-01 , 2.853e-01 , 1.064e-01 , 1.498e-02 , 1.304e-03 , 7.934e-04 },
        { 2.065e-04 , 4.560e-04 , 1.399e-03 , 4.311e-04 , 1.285e-04 , 4.594e-04 , 2.101e-02 , 6.672e-03 , 9.371e-03 , 1.155e-03 , 4.711e-04 , 2.410e-03 , 4.804e-02 , 5.986e-02 , 7.351e-03 , 2.697e-03 , 1.112e-01 , 1.389e-01 , 3.467e-02 , 8.898e-03 , 8.105e-02 , 1.725e-01 , 2.941e-01 , 1.299e-01 , 1.929e-02 , 3.491e-03 },
        { 2.065e-04 , 5.320e-04 , 1.399e-03 , 1.854e-03 , 1.928e-04 , 4.594e-04 , 1.340e-02 , 2.871e-03 , 1.056e-02 , 1.051e-02 , 7.066e-04 , 8.544e-03 , 1.089e-02 , 4.614e-02 , 5.764e-02 , 6.742e-03 , 2.536e-02 , 1.177e-01 , 1.712e-01 , 4.251e-02 , 1.885e-02 , 5.098e-02 , 1.554e-01 , 3.477e-01 , 1.505e-01 , 2.920e-02 },
        { 0.000e+00 , 2.280e-04 , 6.996e-04 , 2.888e-03 , 1.221e-03 , 1.072e-03 , 1.978e-03 , 3.879e-04 , 7.352e-03 , 1.380e-02 , 8.480e-03 , 2.465e-02 , 3.169e-03 , 1.197e-02 , 4.465e-02 , 4.935e-02 , 5.981e-03 , 2.229e-02 , 1.060e-01 , 1.784e-01 , 3.141e-03 , 6.875e-03 , 3.431e-02 , 1.369e-01 , 3.919e-01 , 1.998e-01 },
        { 0.000e+00 , 0.000e+00 , 0.000e+00 , 6.897e-04 , 1.285e-03 , 4.594e-03 , 8.990e-05 , 0.000e+00 , 3.624e-04 , 3.525e-03 , 5.653e-03 , 5.006e-02 , 0.000e+00 , 6.914e-04 , 5.685e-03 , 1.825e-02 , 0.000e+00 , 1.056e-03 , 9.659e-03 , 7.464e-02 , 8.377e-04 , 1.545e-04 , 2.723e-03 , 1.512e-02 , 9.243e-02 , 3.170e-01 }
    };

    const int tile_possession_transforms[NUM_OF_TILES] = {
        25,  // 0
        24,  // 1
        23,  // 2
        22,  // 3
        21,  // 4
        20,  // 5
        11,  // 6
        19,  // 7
        18,  // 8
        17,  // 9
        16,  // 10
        6,   // 11
        15,  // 12
        14,  // 13
        13,  // 14
        12,  // 15
        10,  // 16
        9,   // 17
        8,   // 18
        7,   // 19
        5,   // 20
        4,   // 21
        3,   // 22
        2,   // 23
        1,   // 24
        0    // 25
    };

    void create_tiles();


public:
    Ball();
    Ball(Pitch, bool); // initial possession
    Ball(Pitch, Cart);

    Ball(Ball&);
    Ball(Ball&& b) { *this = b; }

    Ball& operator=(Ball&);
    Ball& operator=(Ball&&);

    Cart getPosition() const;
    void setPosition(Cart);

    Pitch getPitchData() const;
    void setPitchData(Pitch);

    void randomWalk();

    void checkLegalPosition();

    void changeTile(int);
    Tile currentTile() const;
    int currentTileNumber() const;

    void frameUpdate(bool, bool);

    void swap_tiles_for_possession();




};

#endif