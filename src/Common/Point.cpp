#include "Point.h"

Point& Point::adjustToScreen(size_t width, size_t height) {
	x = (x + ((x<0? -x: x) / width + 1) * width) % width;
	y = (y + ((y<0? -y: y) / height + 1) * height) % height;
	return *this;
}

void Point::move(int difx, int dify) {
    x += difx;
    y += dify;
}
void Point::move(Direction d) {
    switch(d) {
    case Direction::East:
        return move(1, 0);
    case Direction::West:
        return move(-1, 0);
    case Direction::South:
        return move(0, 1);
    case Direction::North:
        return move(0, -1);
    case Direction::Stay:
        return move(0, 0);
    }
}
