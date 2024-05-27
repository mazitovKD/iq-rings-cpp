#ifndef UTILS_H
#define UTILS_H

#include <utility>


typedef std::pair<short, short> Cell;

const Cell OUT_OF_BOUNDS = {-1, -1};
const short N_DIRECTIONS = 6;
const short N_ROWS = 4;
const short ROW_LENGHT = 6;
const short DETAIL_SIZE = 3;


template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

short turn(short direction, short rotation) {
    return (6 + direction + rotation) % N_DIRECTIONS;
}

Cell turn(Cell cell, short rotation) {
    short d = cell.first % 2;
    short r0 = rotation, r1= turn(rotation, 1 + 3 * d);
    return Cell({
        cell.first - sgn(r0 - 3) * (r0 > 0),
        cell.second - sgn((float)d - 0.5) * ((r1 < 3)*(r1 % 2) - (r1 >= 3))
    });
}

#endif // UTILS_H
