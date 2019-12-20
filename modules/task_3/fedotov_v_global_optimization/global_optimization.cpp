// Copyright 2019 Fedotov Vlad
#include <mpi.h>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <cmath>
#include <iostream>
#include "../../../modules/task_3/fedotov_v_global_optimization/global_optimization.h"

// global min of ONE argument function.
// fixed variable is need for dimensional reduction method,
// its destination will be clear in getGlobalMinimumOnPlane() function
Point2D getGlobalMinimumOnSegment(double fixedVariable, double leftBorder, double rightBorder, double(*func)(double x, double y), int maxIterationsCount, double r, double accuracy) {
    std::set<Point2D, Point2DComparator> trials;  // calculated points

    double M, maxM, m, R, maxR;
    int countOfTrials = 0;

    // handle borders
    trials.insert(Point2D(leftBorder, func(fixedVariable, leftBorder)));
    countOfTrials++;
    trials.insert(Point2D(rightBorder, func(fixedVariable, rightBorder)));
    countOfTrials++;

    while (countOfTrials < maxIterationsCount) {
        maxM = -999;
        auto iteration = trials.begin();
        iteration++;
        auto previousIteration = trials.begin();

        // calculate max M, which actually is max derivative on intervals
        while (iteration != trials.end()) {
            M = std::abs(static_cast<double>((iteration->y - previousIteration->y) / (iteration->x - previousIteration->x)));
            if (M > maxM)
                maxM = M;
            iteration++;
            previousIteration++;
        }

        // calculate m depending on M
        if (maxM > 0)
            m = r * M;
        else
            m = 1;

        // restore iterators to beginning
        iteration = trials.begin();
        iteration++;
        previousIteration = trials.begin();

        R = -999;

        auto iterationOnMaxR = trials.begin();
        auto previousIterationOnMaxR = trials.begin();

        // calculate R
        while (iteration != trials.end()) {
            R = m*(iteration->x - previousIteration->x) + (std::pow((iteration->y - previousIteration->y), 2) /
            (m * (iteration->x - previousIteration->x))) - 2 * (iteration->y - previousIteration->y);
            if (R > maxR) {
                maxR = R;
                iterationOnMaxR = iteration;
                previousIterationOnMaxR = previousIteration;
            }
            iteration++;
            previousIteration++;
        }

        countOfTrials++;

        // calculating X of new point for trial
        double newPointForTrial = (0.5)*(iterationOnMaxR->x + previousIterationOnMaxR->x) - ((iterationOnMaxR->y - previousIterationOnMaxR->y) / (2 * m));

        // save new counted value of function in new point
        trials.insert(Point2D(newPointForTrial, func(fixedVariable, newPointForTrial)));

        // finish our work when interval is less than required accuracy
        if (iterationOnMaxR->x - previousIterationOnMaxR->x <= accuracy)
            break;
    }

    // find and return calculated min value from set of trials

    auto iterationOnGlobalMin = trials.begin();

    for (auto it=trials.begin(); it != trials.end(); ++it) {
        // std::cout << (it->x) << ' ' << (it->y) << std::endl;
        if (it->y < iterationOnGlobalMin->y) {
            iterationOnGlobalMin = it;
        }
    }

    Point2D globalMin(iterationOnGlobalMin->x, iterationOnGlobalMin->y);

    return globalMin;
}

// global min of TWO argument function
// suppose oX is left-right and oY is bottom-top. oZ is the target function
// we are going to fix oX coordinate, then find minimum on oY segment,
// then choose min of mins
Point3D getGlobalMinimumOnPlane(double xLeftBorder, double xRightBorder, double yBottomBorder, double yTopBorder, double(*func)(double x, double y), int maxIterationsCount, double r, double accuracy) {
    std::set<Point3D, Point3DComparator> trials;  // calculated points

    double M, maxM, m, R, maxR;
    int countOfTrials = 0;

    // handle borders
    // the second param after xLeftBorder will return point2D,
    // and overloaded constructor for  Point3D will be used
    trials.insert(Point3D(xLeftBorder, getGlobalMinimumOnSegment(xLeftBorder, yBottomBorder, yTopBorder, func, maxIterationsCount, r, accuracy)));
    countOfTrials++;
    trials.insert(Point3D(xRightBorder, getGlobalMinimumOnSegment(xRightBorder, yBottomBorder, yTopBorder, func, maxIterationsCount, r, accuracy)));
    countOfTrials++;

    while (countOfTrials < maxIterationsCount) {
        maxM = -999;
        auto iteration = trials.begin();
        iteration++;
        auto previousIteration = trials.begin();

        // calculate max M, which actually is max derivative on intervals
        while (iteration != trials.end()) {
            M = std::abs(static_cast<double>((iteration->z - previousIteration->z) / (iteration->x - previousIteration->x)));
            if (M > maxM)
                maxM = M;
            iteration++;
            previousIteration++;
        }

        // calculate m depending on M
        if (maxM > 0)
            m = r * M;
        else
            m = 1;

        // restore iterators to beginning
        iteration = trials.begin();
        iteration++;
        previousIteration = trials.begin();

        R = -999;

        auto iterationOnMaxR = trials.begin();
        auto previousIterationOnMaxR = trials.begin();

        // calculate R
        while (iteration != trials.end()) {
            R = m*(iteration->x - previousIteration->x) + (std::pow((iteration->z - previousIteration->z), 2) /
            (m * (iteration->x - previousIteration->x))) - 2 * (iteration->z - previousIteration->z);
            if (R > maxR) {
                maxR = R;
                iterationOnMaxR = iteration;
                previousIterationOnMaxR = previousIteration;
            }
            iteration++;
            previousIteration++;
        }

        countOfTrials++;

        // calculating X of new point for trial
        double newPointForTrial = (0.5)*(iterationOnMaxR->x + previousIterationOnMaxR->x) - ((iterationOnMaxR->z - previousIterationOnMaxR->z) / (2 * m));

        // save new counted value of function in new point
        trials.insert(Point3D(newPointForTrial, getGlobalMinimumOnSegment(newPointForTrial, yBottomBorder, yTopBorder, func, maxIterationsCount, r, accuracy)));

        // finish our work when interval is less than required accuracy
        if (iterationOnMaxR->x - previousIterationOnMaxR->x <= accuracy)
            break;
    }

    // find and return calculated min value from set of trials

    auto iterationOnGlobalMin = trials.begin();

    for (auto it=trials.begin(); it != trials.end(); ++it) {
        // std::cout << (it->x) << ' ' << (it->y) << ' ' << (it->z) << std::endl;
        if (it->z < iterationOnGlobalMin->z) {
            iterationOnGlobalMin = it;
        }
    }

    Point3D globalMin(iterationOnGlobalMin->x, iterationOnGlobalMin->y, iterationOnGlobalMin->z);

    return globalMin;
}

double function_1(double x, double y) {
    return x*x + y*y;
}

double function_2(double x, double y) {
    return 0.5*(x-5)*(x-5) + y - 3;
}

double function_3(double x, double y) {
    return (x-5)*(x-5) + (y-3)*(y-3);
}
