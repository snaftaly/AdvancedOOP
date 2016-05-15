#ifndef POINT_H_
#define POINT_H_

#include <ostream>

class Point {
    int x, y;
public:
    Point(int _x, int _y) : x(_x), y(_y) {}
    Point distance(const Point& other)const {
        return Point(x - other.x, y - other.y);
    }
    int getX()const {return x;}
    int getY()const {return y;}
    Point& adjustToScreen(size_t width, size_t height);
    void move(int difx, int dify);
    void move(Direction d);
    // required by map<Point, T>
    bool operator<(const Point& other)const {
        return (x < other.x || (x == other.x && y < other.y));
    }
    friend std::ostream& operator<<(std::ostream& out, const Point& p) {
        return out << "(" << p.x << "," << p.y << ")";
    }
};

#endif /* POINT_H_ */
