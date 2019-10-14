// Copyright 2019 Fedotov Vlad
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./mins_in_matrix_rows.h"


TEST(Parallel_Operations_MPI, Can_Create_Random_Matrix) {
    std::vector<int> matr;

    ASSERT_NO_THROW(matr = getRandomMatrix(3, 3););
}

TEST(Parallel_Operations_MPI, Can_Create_Random_Matrix_Without_Arguments) {
    std::vector<int> matr;

    ASSERT_NO_THROW(matr = getRandomMatrix(););
}

TEST(Parallel_Operations_MPI, Can_Print_Vector) {
    std::vector<int> vector;
    vector = getRandomMatrix();
    ASSERT_NO_THROW(printVector(vector););
}


TEST(Parallel_Operations_MPI, DISABLED_Can_Print_Matrix) {
    std::vector<int> matrix;
    matrix = getRandomMatrix(5, 5);
    ASSERT_NO_THROW(printMatrix(matrix, 5, 5););
}

TEST(Parallel_Operations_MPI, Can_Get_Sequential_Mins_In_Matrix) {
    std::vector<int> matrix, mins;
    matrix = getRandomMatrix(5, 5);
    ASSERT_NO_THROW(mins = getSequentialMinsInMatrix(matrix, 5, 5););
}

TEST(Parallel_Operations_MPI, Can_Get_Parallel_Mins_In_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix, mins;

    if (rank == 0) {
        matrix = getRandomMatrix(5, 5);
    }
    
    mins = getParallelMinsInMatrix(matrix, 5, 5);
    
    ASSERT_NO_THROW();
}

TEST(Parallel_Operations_MPI, Test_Parallel_Mins_In_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix, minsSeq, minsPar;

    if (rank == 0) {
        matrix = getRandomMatrix(5, 5);
        minsSeq = getSequentialMinsInMatrix(matrix, 5, 5);
        minsPar = getParallelMinsInMatrix(matrix, 5, 5);
        ASSERT_EQ(minsSeq, minsPar);
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
/*
    std::vector<int> matrix = getRandomMatrix(5, 5);
    printMatrix(matrix, 5, 5);
    std::vector<int> seqMins = getSequentialMinsInMatrix(matrix, 5, 5);
    std::cout << "Mins: " <<std::endl;
    printVector(seqMins);

    std::vector<int> parMins = getParallelMinsInMatrix(matrix, 5, 5);
    std::cout << "Parallel mins: " <<std::endl;
    printVector(parMins);
*/
    return RUN_ALL_TESTS();
}
