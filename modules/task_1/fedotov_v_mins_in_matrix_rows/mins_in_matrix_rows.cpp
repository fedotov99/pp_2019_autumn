#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <iostream>
#include "../../../modules/task_1/fedotov_v_mins_in_matrix_rows/mins_in_matrix_rows.h"

const static int ROWS_COUNT = 10;
const static int COLS_COUNT = 10;

std::vector<int> matrix[ROWS_COUNT]; // matrix of ROWS_COUNT rows, that's represented as array of vectors<int>
int mins[ROWS_COUNT]; // array of mins of each row

std::vector<int> getRandomVector() { // default size is COLS_COUNT
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(COLS_COUNT);
    for (int  i = 0; i < COLS_COUNT; i++) { vec[i] = gen() % 100; }
    return vec;
}

void getRandomMatrix() {
    for (int i = 0; i < ROWS_COUNT; i++) {
        matrix[i] = getRandomVector(); // in this task we assume that matrix can be only of equal rows
    }
}

void printVector(std::vector<int> vec) {
    int row_size = vec.size();
    for (int i = 0; i < row_size; i++)
        std::cout << vec[i] << " ";
    std::cout << std::endl;
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
    int row_size = row.size();
    for (int i = 0; i < row_size; i++) {
        min = std::min(min, row[i]);
    }
    return min;
}

int getParallelMinInRow(std::vector<int> row) {
    int size, rank;
    int row_size = row.size();
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // split our row (vector) into parts and send the addresses of each part
    // to other processes
    const int delta = row_size / size;
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