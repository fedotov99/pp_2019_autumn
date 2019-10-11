// Copyright 2019 Fedotov Vlad
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./mins_in_matrix_rows.h"

TEST(Parallel_Operations_MPI, Can_Create_Random_Vector) {
    std::vector<int> row;

    ASSERT_NO_THROW(row = getRandomVector(););
}

TEST(Parallel_Operations_MPI, Can_Create_Random_Matrix) {
    ASSERT_NO_THROW(getRandomMatrix(););
}

TEST(Parallel_Operations_MPI, Can_Print_Vector) {
    std::vector<int> vector;
    vector = getRandomVector();
    ASSERT_NO_THROW(printVector(vector););
}


TEST(Parallel_Operations_MPI, Can_Print_Matrix) {
    getRandomMatrix();
    ASSERT_NO_THROW(printMatrix(););
}

TEST(Parallel_Operations_MPI, Can_Get_Sequential_Mins_In_Matrix_Unitialized) {
    ASSERT_NO_THROW(getSequentialMinsInMatrix(););
}

TEST(Parallel_Operations_MPI, Can_Get_Sequential_Mins_In_Matrix_Initialized) {
    ASSERT_NO_THROW(getSequentialMinsInMatrix(););
}

TEST(Parallel_Operations_MPI, Test_Parallel_Min_In_Row) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> row;

    if (rank == 0) {
        row = getRandomVector();
    }

    int min;
    min = getParallelMinInRow(row);

    if (rank == 0) {
        int reference_min = getSequentialMinInRow(row);
        ASSERT_EQ(reference_min, min);
    }
}

TEST(Parallel_Operations_MPI, Test_Parallel_Mins_In_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> row;

    if (rank == 0) {
        row = getRandomVector();
    }

    int min;
    min = getParallelMinInRow(row);

    if (rank == 0) {
        int reference_min = getSequentialMinInRow(row);
        ASSERT_EQ(reference_min, min);
    }
}

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
}