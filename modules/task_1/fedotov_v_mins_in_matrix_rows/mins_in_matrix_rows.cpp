#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <iostream>
#include "../../../modules/task_1/fedotov_v_mins_in_matrix_rows/mins_in_matrix_rows.h"

std::vector<int> getRandomVector(int sz = COLS_COUNT) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(sz);
    for (int  i = 0; i < sz; i++) { vec[i] = gen() % 100; }
    return vec;
}

void getRandomMatrix() {
    for (int i = 0; i < ROWS_COUNT; i++) {
        matrix[i] = getRandomVector(); // in this task we assume that matrix can be only of equal rows
    }
}

void printMatrix() {
    for (int i = 0; i < ROWS_COUNT; i++) {
        for (int j = 0; j < COLS_COUNT; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int getSequentialMinInRow(std::vector<int> row) {
    int min = row[0];
    for (int i = 0; i < row.size(); i++) {
        min = std::min(min, row[i]);
    }
}

int getParallelMinInRow(std::vector<int> row) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // split our row (vector) into parts and send the addresses of each part
    // to other processes
    const int delta = row.size() / size;
    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(&row[0] + proc * delta, delta,
                        MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }

    // now we receive these parts of row (vector) in every process
    std::vector<int> part_of_row(delta);
    if (rank == 0) {
        part_of_row = std::vector<int>(row.begin(),
                                     row.begin() + delta);
    } else {
        MPI_Status status;
        MPI_Recv(&part_of_row[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    int part_min = getSequentialMinInRow(part_of_row);
    int min = INT32_MAX;
    MPI_Reduce(&part_min, &min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    return min;
}