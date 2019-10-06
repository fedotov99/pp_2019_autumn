// Copyright 2019 Fedotov Vlad
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./mins_in_matrix_rows.h"

TEST(Parallel_Operations_MPI, Test_Min) {
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