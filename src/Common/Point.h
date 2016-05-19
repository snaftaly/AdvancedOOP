#ifndef POINT_H_
#define POINT_H_

#include <ostream>
#include "Direction.h"

class Point {
    long x, y;
public:
    Point(): x(-1), y(-1){}
    Point(long _x, long _y) : x(_x), y(_y) {}
    Point distance(const Point& other)const {
        return Point(x - other.x, y - other.y);
    }
    long getX()const {return x;}
    long getY()const {return y;}
    Point& adjustToScreen(size_t width, size_t height);
    void move(long difx, long dify);
    void move(Direction d);
    // required by map<Point, T>
    bool operator<(const Point& other) const{
        return (x < other.x || (x == other.x && y < other.y));
    }
    friend std::ostream& operator<<(std::ostream& out, const Point& p) {
        return out << "(" << p.x << "," << p.y << ")";
    }
    friend bool operator==(const Point& lhs, const Point& rhs){
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};

#endif /* POINT_H_ */
