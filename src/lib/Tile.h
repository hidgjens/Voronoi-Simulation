#ifndef TILE_H
#define TILE_H

#include "Cart.h"
#include<memory>


class Tile{
    
private:
    int tile_number;
    Cart centre_position;
    int corners = 4;
    std::unique_ptr<Cart[]> corner_positions;

    double area;

public:
    Tile();
    Tile(int, Cart, Cart); // number, bottom left, top right edges

    Tile(Tile&);
    Tile(Tile&& t) { *this = std::move(t); }

    Tile& operator=(Tile&);
    Tile& operator=(Tile&&);

    Cart getTopLeftCorner() const;
    Cart getTopRightCorner() const;
    Cart getBottomLeftCorner() const;
    Cart getBottomRightCorner() const;
    Cart getCentre() const;

    Cart getCorner(int) const;

    void setTileNumber(int);
    int getTileNumber() const;


};

#endif