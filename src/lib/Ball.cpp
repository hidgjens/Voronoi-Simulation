#include "Ball.h"

Ball::Ball() : pos(0.0, 0.0) , actual_num_of_tiles(NUM_OF_TILES) {}

Ball::Ball(Pitch p, bool init_poss) : pitch_data(p) , pos(0.0, 0.0), actual_num_of_tiles(NUM_OF_TILES) { 
    tiles = std::make_unique<Tile[]>(actual_num_of_tiles); 
    create_tiles();

    if (!init_poss) {
        // initial possession is Away, need to swap
        swap_tiles_for_possession();
    }

}

Ball::Ball(Pitch p, Cart r) : pitch_data(p) , pos(r), actual_num_of_tiles(NUM_OF_TILES) { tiles = std::make_unique<Tile[]>(actual_num_of_tiles); }

void Ball::create_tiles(){
    /* 
        T E D I O U S 
    */

    double X_TICKS[7] = { -52.5, -35.0, -17.5, 0.0, 17.5, 35.0, 52.5 };
    double Y_TICKS[6] = { -34.0, -20.4, -6.8, 6.8, 20.4, 34.0 };
    Cart blc, trc;

    // tile 0 [0, 4], [1, 5]
    blc = Cart(X_TICKS[0], Y_TICKS[4]);
    trc = Cart(X_TICKS[1], Y_TICKS[5]);
    tiles[0] = Tile(0, blc, trc);

    // tile 1 [1, 4], [2, 5]
    blc = Cart(X_TICKS[1], Y_TICKS[4]);
    trc = Cart(X_TICKS[2], Y_TICKS[5]);
    tiles[1] = Tile(1, blc, trc);

    // tile 2 [2, 4], [3, 5]
    blc = Cart(X_TICKS[2], Y_TICKS[4]);
    trc = Cart(X_TICKS[3], Y_TICKS[5]);
    tiles[2] = Tile(2, blc, trc);

    // tile 3 [3, 4], [4, 5]
    blc = Cart(X_TICKS[3], Y_TICKS[4]);
    trc = Cart(X_TICKS[4], Y_TICKS[5]);
    tiles[3] = Tile(3, blc, trc);

    // tile 4 [4, 4], [5, 5]
    blc = Cart(X_TICKS[4], Y_TICKS[4]);
    trc = Cart(X_TICKS[5], Y_TICKS[5]);
    tiles[4] = Tile(4, blc, trc);

    // tile 5 [5, 4], [6, 5]
    blc = Cart(X_TICKS[5], Y_TICKS[4]);
    trc = Cart(X_TICKS[6], Y_TICKS[5]);
    tiles[5] = Tile(5, blc, trc);

    // tile 6 [0, 1], [1, 4]
    blc = Cart(X_TICKS[0], Y_TICKS[1]);
    trc = Cart(X_TICKS[1], Y_TICKS[4]);
    tiles[6] = Tile(6, blc, trc);

    // tile 7 [1, 3], [2, 4]
    blc = Cart(X_TICKS[1], Y_TICKS[3]);
    trc = Cart(X_TICKS[2], Y_TICKS[4]);
    tiles[7] = Tile(7, blc, trc);

    // tile 8 [2, 3], [3, 4]
    blc = Cart(X_TICKS[2], Y_TICKS[3]);
    trc = Cart(X_TICKS[3], Y_TICKS[4]);
    tiles[8] = Tile(8, blc, trc);

    // tile 9 [3, 3], [4, 4]
    blc = Cart(X_TICKS[3], Y_TICKS[3]);
    trc = Cart(X_TICKS[4], Y_TICKS[4]);
    tiles[9] = Tile(9, blc, trc);

    // tile 10 [4, 3], [5, 4]
    blc = Cart(X_TICKS[4], Y_TICKS[3]);
    trc = Cart(X_TICKS[5], Y_TICKS[4]);
    tiles[10] = Tile(10, blc, trc);

    // tile 11 [5, 1], [6, 4]
    blc = Cart(X_TICKS[5], Y_TICKS[1]);
    trc = Cart(X_TICKS[6], Y_TICKS[4]);
    tiles[11] = Tile(11, blc, trc);

    // tile 12 [1, 2], [2, 3]
    blc = Cart(X_TICKS[1], Y_TICKS[2]);
    trc = Cart(X_TICKS[2], Y_TICKS[3]);
    tiles[12] = Tile(12, blc, trc);

    // tile 13 [2, 2], [3, 3]
    blc = Cart(X_TICKS[2], Y_TICKS[2]);
    trc = Cart(X_TICKS[3], Y_TICKS[3]);
    tiles[13] = Tile(13, blc, trc);

    // tile 14 [3, 2], [4, 3]
    blc = Cart(X_TICKS[3], Y_TICKS[2]);
    trc = Cart(X_TICKS[4], Y_TICKS[3]);
    tiles[14] = Tile(14, blc, trc);

    // tile 15 [4, 2], [5, 3]
    blc = Cart(X_TICKS[4], Y_TICKS[2]);
    trc = Cart(X_TICKS[5], Y_TICKS[3]);
    tiles[15] = Tile(15, blc, trc);

    // tile 16 [1, 1], [2, 2]
    blc = Cart(X_TICKS[1], Y_TICKS[1]);
    trc = Cart(X_TICKS[2], Y_TICKS[2]);
    tiles[16] = Tile(16, blc, trc);

    // tile 17 [2, 1], [3, 2]
    blc = Cart(X_TICKS[2], Y_TICKS[1]);
    trc = Cart(X_TICKS[3], Y_TICKS[2]);
    tiles[17] = Tile(17, blc, trc);

    // tile 18 [3, 1], [4, 2]
    blc = Cart(X_TICKS[3], Y_TICKS[1]);
    trc = Cart(X_TICKS[4], Y_TICKS[2]);
    tiles[18] = Tile(18, blc, trc);

    // tile 19 [4, 1], [5, 2]
    blc = Cart(X_TICKS[4], Y_TICKS[1]);
    trc = Cart(X_TICKS[5], Y_TICKS[2]);
    tiles[19] = Tile(19, blc, trc);

    // tile 20 [0, 0], [1, 1]
    blc = Cart(X_TICKS[0], Y_TICKS[0]);
    trc = Cart(X_TICKS[1], Y_TICKS[1]);
    tiles[20] = Tile(20, blc, trc);

    // tile 21 [1, 0], [2, 1]
    blc = Cart(X_TICKS[1], Y_TICKS[0]);
    trc = Cart(X_TICKS[2], Y_TICKS[1]);
    tiles[21] = Tile(21, blc, trc);

    // tile 22 [2, 0], [3, 1]
    blc = Cart(X_TICKS[2], Y_TICKS[0]);
    trc = Cart(X_TICKS[3], Y_TICKS[1]);
    tiles[22] = Tile(22, blc, trc);

    // tile 23 [3, 0], [4, 1]
    blc = Cart(X_TICKS[3], Y_TICKS[0]);
    trc = Cart(X_TICKS[4], Y_TICKS[1]);
    tiles[23] = Tile(23, blc, trc);

    // tile 24 [4, 0], [5, 1]
    blc = Cart(X_TICKS[4], Y_TICKS[0]);
    trc = Cart(X_TICKS[5], Y_TICKS[1]);
    tiles[24] = Tile(24, blc, trc);

    // tile 25 [5, 0], [6, 1]
    blc = Cart(X_TICKS[5], Y_TICKS[0]);
    trc = Cart(X_TICKS[6], Y_TICKS[1]);
    tiles[25] = Tile(25, blc, trc);
}

void Ball::swap_tiles_for_possession(){
    std::unique_ptr<Tile[]> temp_tiles = std::make_unique<Tile[]>(actual_num_of_tiles);
    // copy tiles into temp
    for (int i{0}; i < actual_num_of_tiles; i++) {
        temp_tiles[i] = tiles[i];
    }
    /*
    Now take temp tiles, find out what their new index is, 
    and place them into their new position with that index
    */
    Tile t;
    int n_idx;
    for (int i{0}; i < actual_num_of_tiles; i++) {
        t = temp_tiles[i];
        n_idx = tile_possession_transforms[i];
        t.setTileNumber(n_idx);
        tiles[n_idx] = t;
    }
    // change current tile index
    n_idx = tile_possession_transforms[tile_index];
    std::cout << "Possession swapped " << tile_index << " --> " << n_idx << std::endl;
    tile_index = n_idx;

}

Ball::Ball(Ball& b) { 
    pos = b.pos;
    actual_num_of_tiles = b.actual_num_of_tiles;
    pitch_data = b.pitch_data;
    max_multiplier = b.max_multiplier;
    min_multiplier = b.min_multiplier;

    tile_index = b.tile_index;

    tiles = std::make_unique<Tile[]>(actual_num_of_tiles);

    for (int i{0}; i < actual_num_of_tiles; i++) {
        tiles[i] = b.tiles[i];
    }
}

Ball& Ball::operator=(Ball& b) {
    if (&b == this) {
        return *this;
    }

    pos = b.pos;
    pitch_data = b.pitch_data;
    max_multiplier = b.max_multiplier;
    min_multiplier = b.min_multiplier;

    tile_index = b.tile_index;

    tiles = std::make_unique<Tile[]>(actual_num_of_tiles);

    for (int i{0}; i < actual_num_of_tiles; i++) {
        tiles[i] = b.tiles[i];
    }
}

Ball& Ball::operator=(Ball&& b) {
    pos = b.pos;
    pitch_data = b.pitch_data;
    max_multiplier = b.max_multiplier;
    min_multiplier = b.min_multiplier;

    tile_index = b.tile_index;

    tiles = std::move(b.tiles);
    b.actual_num_of_tiles = 0;


}

Cart Ball::getPosition() const { return pos; }
void Ball::setPosition(Cart r) { pos = r; }

Pitch Ball::getPitchData() const { return pitch_data; }
void Ball::setPitchData(Pitch p) { pitch_data = p; }

void Ball::randomWalk() {
    double rand_direction = ((double) rand() / RAND_MAX) * 2 * M_PI;
    double rand_distance = ((double) rand() / RAND_MAX) * (max_multiplier - min_multiplier) + min_multiplier; // [0,1] -> [min, max]

    Cart dPos(rand_distance * player_step_size * cos(rand_direction), rand_distance * player_step_size * sin(rand_direction));

    pos += dPos;
    checkLegalPosition();
}

void Ball::checkLegalPosition() {
    double x_cmpt = pos.xComp();
    double y_cmpt = pos.yComp();

    if (x_cmpt > pitch_data.getXlim()) {
        x_cmpt = pitch_data.getXlim();
    } else if (x_cmpt < -1.0 * pitch_data.getXlim()) {
        x_cmpt = -1.0 * pitch_data.getXlim();
    }

    if (y_cmpt > pitch_data.getYlim()) {
        y_cmpt = pitch_data.getYlim();
    } else if (y_cmpt < -1.0 * pitch_data.getYlim()) {
        y_cmpt = -1.0 * pitch_data.getYlim();
    }
}

void Ball::changeTile(int i) {
    if (i < actual_num_of_tiles) {
        tile_index = i;
        pos = tiles[i].getCentre();
    }
}

Tile Ball::currentTile() const { return tiles[tile_index]; }
int Ball::currentTileNumber() const { return tile_index; }

void Ball::frameUpdate(bool poss_change, bool passing) {

    if (poss_change) {
        swap_tiles_for_possession();
    }

    if (passing) {
        int initial_state = tile_index;

        double transition_array[actual_num_of_tiles];

        // std::cout << "actual number of tiles " << actual_num_of_tiles << std::endl;

        for (int i{0}; i < actual_num_of_tiles; i++) {
            transition_array[i] = stochastic_matrix[i][initial_state];
        }

        // std::cout << "here" << std::endl;

        double cumulative_array[actual_num_of_tiles];

        // std::cout << "here" << std::endl;

        cumulative_array[0] = transition_array[0];

        // std::cout << "here" << std::endl;


        for (int i{1}; i < actual_num_of_tiles; i++) {
            cumulative_array[i] = cumulative_array[i-1] + transition_array[i];
            // std::cout << cumulative_array[i] << std::endl;
        }

        // std::cout << "here" << std::endl;


        double random_number = ((double) rand() / RAND_MAX); // random number [0,1]
        double test_value;
        int new_tile;

        for (int i{0}; i < actual_num_of_tiles; i++) {
            // std::cout << i << " here" << std::endl;

            test_value = cumulative_array[i];

            if (random_number < test_value) {
                // this is the chosen tile
                new_tile = i;
                break; // DONT CARRY ON WITH LOOP
            }
        }
        // std::cout << "here" << std::endl;

        changeTile(new_tile);
        std::cout << "Transition " << initial_state << " -> " << new_tile << " with transition rate " << transition_array[new_tile] << std::endl;
    }
}