// Copyright 2019 Fedotov Vlad
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./global_optimization.h"

TEST(Parallel_Operations_MPI, Test_Can_Get_Global_Minimum_Sequentially_F1) {
    double(*func)(double, double) = function_1;
    double xLeftBorder = -10, xRightBorder = 10;
    double yBottomBorder = -10, yTopBorder = 10;
    double r = 2, accuracy = 0.01;
    int maxIterationsCount = 100;

    Point3D trueGlobalMin(0, 0, 0);
    Point3D countedGlobalMin = getGlobalMinimumOnPlane(xLeftBorder, xRightBorder,
    yBottomBorder, yTopBorder, func, maxIterationsCount, r, accuracy);

    EXPECT_EQ(1, std::abs(countedGlobalMin.x - trueGlobalMin.x) <= accuracy);
    EXPECT_EQ(1, std::abs(countedGlobalMin.y - trueGlobalMin.y) <= accuracy);
}

// TEST(Parallel_Operations_MPI, Test_Philosophers_Ended_Their_Job) {
//     int rank, size;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &size);
    
//     }
// }

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);

    return RUN_ALL_TESTS();

    // double(*func)(double, double) = function_2;
    // // Point2D globalMinOnSegment = getGlobalMinimumOnSegment(-10, 10, func, 100, 2, 0.01);
    // Point3D globalMinOnPlane = getGlobalMinimumOnPlane(-10, 10, -10, 10, func, 100, 2, 0.01);
    // std::cout << "Global min is in: " << globalMinOnPlane.x << " " << globalMinOnPlane.y << "value " << globalMinOnPlane.z << std::endl;
}
