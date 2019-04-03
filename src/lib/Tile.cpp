#include "Tile.h"

Tile::Tile() {
    tile_number = 1;
    corner_positions = std::make_unique<Cart[]>(corners);
    Cart blc = Cart(-1.0, -1.0);
    Cart trc = Cart(1.0, 1.0);
    corner_positions[0] = blc; corner_positions[2] = trc;

    double x_length = trc.xComp() - blc.xComp();
    double y_length = trc.yComp() - blc.yComp();

    area = x_length * y_length;

    Cart brc = blc + Cart(x_length, 0.0);
    Cart tlc = blc + Cart(0.0, y_length);

    corner_positions[1] = tlc; corner_positions[3] = brc;

    centre_position = (tlc + trc + blc + brc) / 4.0;
}

Tile::Tile(int i, Cart blc, Cart trc) : tile_number(i) {
    corner_positions = std::make_unique<Cart[]>(corners);
    corner_positions[0] = blc; corner_positions[2] = trc;

    double x_length = trc.xComp() - blc.xComp();
    double y_length = trc.yComp() - blc.yComp();

    area = x_length * y_length;

    Cart brc = blc + Cart(x_length, 0.0);
    Cart tlc = blc + Cart(0.0, y_length);

    corner_positions[1] = tlc; corner_positions[3] = brc;

    centre_position = (tlc + trc + blc + brc) / 4.0;
}

Tile::Tile(Tile& t) {
    corners = t.corners;
    tile_number = t.tile_number;
    area = t.area;
    centre_position = t.centre_position;

    corner_positions = std::make_unique<Cart[]>(corners);
    for (int i{0}; i < corners; i++ ) {
        corner_positions[i] = t.corner_positions[i];
    }
}

Tile& Tile::operator=(Tile& t) {
    if (&t == this) {
        return *this;
    }
    corners = t.corners;
    tile_number = t.tile_number;
    area = t.area;
    centre_position = t.centre_position;

    corner_positions = std::make_unique<Cart[]>(corners);
    for (int i{0}; i < corners; i++ ) {
        corner_positions[i] = t.corner_positions[i];
    }

    return *this;
}

Tile& Tile::operator=(Tile&& t) {
    if (&t == this) {
        return *this;
    }
    corners = t.corners;
    tile_number = t.tile_number;
    area = t.area;
    centre_position = t.centre_position;

    corner_positions = std::move(t.corner_positions);
    t.corners = 0;

    return *this;
}

Cart Tile::getCorner(int i) const { return corner_positions[i]; }
Cart Tile::getTopLeftCorner() const { return getCorner(1); }
Cart Tile::getTopRightCorner() const { return getCorner(2); }
Cart Tile::getBottomLeftCorner() const { return getCorner(0); }
Cart Tile::getBottomRightCorner() const { return getCorner(3); }
Cart Tile::getCentre() const { return centre_position; }

void Tile::setTileNumber(int tn) {tile_number = tn; }
int Tile::getTileNumber() const { return tile_number; }