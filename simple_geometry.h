#ifndef __SIMPLE_GEOMETRY
#define __SIMPLE_GEOMETRY

#ifndef EPS
#define EPS 1e-10
#endif

#define sqr(x) ((x) * (x))

struct Point {
    double x, y;
    Point operator+(const Point& p) const { return {x + p.x, y + p.y}; }
    Point operator-(const Point& p) const { return {x - p.x, y - p.y}; }
    Point operator*(const double d) const { return {x * d, y * d}; }
    Point rotate() const { return {y, -x}; }
    double operator*(const Point& p) const { return x * p.x + y * p.y; }
    double operator^(const Point& p) const { return x * p.y - y * p.x; }
    double dist() const { return sqrt(x * x + y * y); }
};

struct Line {
    double a, b, c;
    Line(const Point& p1, const Point& p2) {
        a = p1.y - p2.y, b = p2.x - p1.x;
        c = - a * p1.x - b * p1.y;
        double d = sqrt(sqr(a) + sqr(b));
        a /= d, b /= d, c /= d;
    }
    Line(const Point& p0, double angle) {
        a = sin(angle), b = cos(angle);
        c = - a * p0.x - b * p0.y;
        double d = sqrt(sqr(a) + sqr(b));
        a /= d, b /= d, c /= d;
    }
    bool operator||(const Line& l) const { return fabs(a * l.b - l.a * b) < EPS; }
    double dist(const Point& p) const { return fabs(a * p.x + b * p.y + c); }
    Point operator^(const Line& l) const {
        return {(l.c * b - c * l.b) / (a * l.b - l.a * b),
                (l.c * a - c * l.a) / (l.a * b - a * l.b)};
    }
    Point projection(const Point& p) const {
        return p - Point{a, b} * (a * p.x + b * p.y + c);
    }
};

struct Circle {
    Point c;
    double r;
    Circle(const Point& c, double r) : c(c), r(r) {}
    Circle(const Point& a, const Point& b, const Point& c) {
        Point p1 = (a + b) * 0.5, p2 = (a + c) * 0.5;
        Point q1 = p1 + (b - a).rotate(), q2 = p2 + (c - a).rotate();
        this->c = Line(p1, q1) ^ Line(p2, q2);
        r = (a - this->c).dist();
    }
};

inline bool on_segment(const Point& p1, const Point& p2, const Point& x, bool strictly) {
    if (fabs((p1 - x) ^ (p2 - x)) > EPS) 
        return false;
    return (p1 - x) * (p2 - x) < (strictly ? - EPS : EPS);
}

// in case intersection is not a segment
inline bool intersect_segments(const Point& p1, const Point& p2, const Point& q1, const Point& q2, Point& x) {
    Line l1(p1, p2), l2(q1, q2);
    if (l1 || l2) return false;
    x = l1 ^ l2;
    return on_segment(p1, p2, x, false) && on_segment(q1, q2, x, false);
}

// in case circles are not equal
inline bool intersect_circles(const Circle& c1, const Circle& c2, Point& p1, Point& p2) {
    double d = (c2.c - c1.c).dist();
    if (d > c1.r + c2.r + EPS || d < fabs(c1.r - c2.r) - EPS)
        return false;
    double cosa = (sqr(d) + sqr(c1.r) - sqr(c2.r)) / (2 * c1.r * d);
    double l = c1.r * cosa, h = sqrt(sqr(c1.r) - sqr(l));
    Point v = (c2.c - c1.c) * (1 / d), p = c1.c + v * l;
    p1 = p + v.rotate() * h, p2 = p - v.rotate() * h;
    return true;
}

inline bool intersect_circle_and_line(const Circle& c, const Line& l, Point& p1, Point& p2) {
    double d = l.dist(c.c);
    if (d > c.r + EPS)
        return false;
    Point p = l.projection(c.c);
    Point n{l.b, -l.a};
    double h = sqrt(sqr(c.r) - sqr(l.dist(c.c)));
    p1 = p + n * h, p2 = p - n * h;
    return true;
}

#endif
