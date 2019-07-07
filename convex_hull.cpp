// convex hull begins

struct Point {
    int x, y;
    Point operator-(const Point& p) const { return {x - p.x, y - p.y}; }
    int64_t operator^(const Point& p) const { return x * 1ll * p.y - y * 1ll * p.x; }
    int64_t dist() const { return x * 1ll * x + y * 1ll * y; }
    bool operator<(const Point& p) const { return x != p.x ? x < p.x : y < p.y; }
};

// all points on convex hull are included
vector<Point> convex_hull(vector<Point> pt) {
    int n = pt.size();
    Point p0 = *std::min_element(pt.begin(), pt.end());
    std::sort(pt.begin(), pt.end(), [&p0](const Point& a, const Point& b) {
    int64_t cp = (a - p0) ^ (b - p0);
    return cp != 0 ? cp > 0 : (a - p0).dist() < (b - p0).dist();
    });


    int i = n - 1;
    for (; i > 0 && ((pt[i] - p0) ^ (pt[i - 1] - p0)) == 0; --i);
    std::reverse(pt.begin() + i, pt.end());

    vector<Point> ch;
    for (auto& p : pt) {
        while (ch.size() > 1) {
            auto& p1 = ch[(int) ch.size() - 1];
            auto& p2 = ch[(int) ch.size() - 2];
            int64_t cp = (p1 - p2) ^ (p - p1);
            if (cp >= 0) break;
            ch.pop_back();
        }
        ch.push_back(p);
    }

    return ch;
}

// convex hull ends
