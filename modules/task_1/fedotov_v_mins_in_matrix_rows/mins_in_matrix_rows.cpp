#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <ctime>
#include <iostream>
#include "../../../modules/task_1/fedotov_v_mins_in_matrix_rows/mins_in_matrix_rows.h"

const int ROWS_COUNT = 10;
const int COLS_COUNT = 10;

std::vector<int> matrix[ROWS_COUNT]; // matrix of ROWS_COUNT rows, that's represented as array of vectors<int>
bool matrixInitialized = false;
int mins[ROWS_COUNT]; // array of mins in each row of matrix
int minsByParallel[ROWS_COUNT]; // this array is useful for comparison of sequantial and parallel results

std::vector<int> getRandomVector(int n) { // default size is COLS_COUNT
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(COLS_COUNT);
    for (int  i = 0; i < COLS_COUNT; i++) { vec[i] = (gen() + n*2) % 100; }
    return vec;
}

void getRandomMatrix() {
    for (int i = 0; i < ROWS_COUNT; i++) {
        matrix[i] = getRandomVector(i); // in this task we assume that matrix can be only of equal rows
    }
    matrixInitialized = true;
}

void printVector(std::vector<int> vec) {
    int row_size = vec.size();
    for (int i = 0; i < row_size; i++)
        std::cout << vec[i] << " ";
    std::cout << std::endl;
}

void printMatrix() {
    if (matrixInitialized) {
        for (int i = 0; i < ROWS_COUNT; i++) {
            for (int j = 0; j < COLS_COUNT; j++) {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    else std::cout << "Matrix is not initialized " << std::endl;

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

void getSequentialMinsInMatrix() {
    if(!matrixInitialized)
        getRandomMatrix();
    for (int i = 0; i < ROWS_COUNT; i++) {
        mins[i] = getSequentialMinInRow(matrix[i]);
    }
}

/*
void getParallelMinsInMatrix() { // distribute rows of matrix between processes
    if(!matrixInitialized)
        getRandomMatrix();
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // send the addresses of each row to other processes
    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            std::cout << "in cycle" << std::endl;
            //MPI_Send(&matrix[0] + proc * sizeof(std::vector<int>), ROWS_COUNT,
            //            MPI_INT, proc, 0, MPI_COMM_WORLD); 
            MPI_Send(&matrix[proc], ROWS_COUNT - 1,
                        MPI_INT, proc, 0, MPI_COMM_WORLD);
            std::cout << "process 0 sended to " << proc << std::endl;
        }
        std::cout << "process 0 sended" << std::endl;
    }

    // receive these addresses of rows in every process
    std::vector<int> localMatrixRow;
    if (rank == 0) {
        localMatrixRow = std::vector<int>(matrix[0]);
        minsByParallel[0] = getParallelMinInRow(localMatrixRow);
        std::cout << "process 0 received" << std::endl;
    } else {
        std::cout << "rank " << rank << std::endl;
        MPI_Status status;
        MPI_Recv(&localMatrixRow[0], sizeof(std::vector<int>), 
                    MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        std::cout << "process" << rank << " received" << std::endl;
    }
    minsByParallel[rank] = getParallelMinInRow(localMatrixRow);
}
*/

void getParallelMinsInMatrix() {
    if(!matrixInitialized)
        getRandomMatrix();
    for (int i = 0; i < ROWS_COUNT; i++) {
        minsByParallel[i] = getParallelMinInRow(matrix[i]);
    }
}

void printMins() {
    for (int i = 0; i < ROWS_COUNT; i++) {
        std::cout << mins[i] << " ";
    }
    std::cout << std::endl;
}


void printMinsByParallel() {
    for (int i = 0; i < ROWS_COUNT; i++) {
        std::cout << minsByParallel[i] << " ";
    }
    std::cout << std::endl;
}
