// Copyright 2019 Fedotov Vlad
#ifndef MODULES_TASK_3_FEDOTOV_V_GLOBAL_OPTIMIZATION_GLOBAL_OPTIMIZATION_H_
#define MODULES_TASK_3_FEDOTOV_V_GLOBAL_OPTIMIZATION_GLOBAL_OPTIMIZATION_H_

class Point2D {
 public:
    double x;
    double y;

    Point2D(double _x, double _y) {
        x = _x;
        y = _y;
    }

    friend bool operator<(const Point2D& firstPoint, const Point2D& secondPoint){
        return firstPoint.x < secondPoint.x;
    }
};

class Point3D : Point2D {
 public:
    double z;

    Point3D(double _x, double _y, double _z) : Point2D(_x, _y) {
        z = _z;
    }

    // friend bool operator<(const Point3D& firstPoint, const Point3D& secondPoint){
    //     return firstPoint.x < secondPoint.x;
    // }
};

Point2D getGlobalMinimumOnSegment(double leftBorder, double rightBorder, double(*func)(double x), int maxIterationsCount, double r, double accuracy);

double function(double x) {
    return x*x;
}

struct Point2DComparator {
    bool operator()(Point2D const& p1, Point2D const& p2) {
        return p1.x < p2.x;
    }
};


#endif  // MODULES_TASK_3_FEDOTOV_V_GLOBAL_OPTIMIZATION_GLOBAL_OPTIMIZATION_H_
