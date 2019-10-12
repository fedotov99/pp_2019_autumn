// Copyright 2019 Fedotov Vlad
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <ctime>
#include <iostream>
#include "../../../modules/task_1/fedotov_v_mins_in_matrix_rows/mins_in_matrix_rows.h"

const int ROWS_COUNT = 5;
const int COLS_COUNT = 5;

std::vector<int> matrix;  // matrix of ROWS_COUNT rows, that's represented as vector<int>
bool matrixInitialized = false;
int minsBySequential[ROWS_COUNT];  // array of mins in each row of matrix, calculated sequentially
int minsByParallel[ROWS_COUNT];  // this array is useful for comparison of sequantial and parallel results

/*
std::vector<int> getRandomVector(int n) {  // default size is COLS_COUNT
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(COLS_COUNT);
    for (int  i = 0; i < COLS_COUNT; i++) { vec[i] = (gen() + n*2) % 100; }
    return vec;
}

void getRandomMatrix() {
    for (int i = 0; i < ROWS_COUNT; i++) {
        matrix[i] = getRandomVector(i);  // in this task we assume that matrix can be only of equal rows
    }
    matrixInitialized = true;
}
*/

void getRandomMatrix() {
    matrix = std::vector<int>(ROWS_COUNT * COLS_COUNT);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < ROWS_COUNT * COLS_COUNT; i++) {
        matrix[i] = gen() % 100;
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
        for (int i = 1; i <= ROWS_COUNT * COLS_COUNT; i++) {
            std::cout << matrix[i-1] << " ";
            if (i % ROWS_COUNT == 0)  // move to new row
                std::cout << std::endl;
        }
    } else {
        std::cout << "Matrix is not initialized " << std::endl;
    }
}

int getSequentialMinInRow(int m) {
    int startIndex = m * COLS_COUNT;
    int endIndex = startIndex + COLS_COUNT;
    int min = matrix[startIndex];
    for (int i = startIndex; i < endIndex; i++) {
        min = std::min(min, matrix[i]);
    }
    return min;
}

void getSequentialMinsInMatrix() {
    if (!matrixInitialized)
        getRandomMatrix();
    for (int i = 0; i < ROWS_COUNT; i++) {
        minsBySequential[i] = getSequentialMinInRow(i);
    }
}

int getSequentialMinInVec(std::vector<int> vec) {
    int min = vec[0];
    for (unsigned int i = 0; i < vec.size(); i++) {
        min = std::min(min, vec[i]);
    }
    return min;
}

std::vector<int> getParallelMinsInMatrix() {
    if(!matrixInitialized)
        getRandomMatrix();
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int delta = (ROWS_COUNT / size) * COLS_COUNT;  // number of elements
    // per process
    // if ROWS_COUNT % size != 0, then we have to consider this case in 
    // process 0.
    // It means that process 0 will take:
    // (ROWS_COUNT % size)*COLS_COUNT + delta elements in matrix

    // send the addresses to other processes
    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(&matrix[(ROWS_COUNT % size)*COLS_COUNT] + proc * delta, delta,
                        MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }

    // create local_vec for treating parts of matrix in every process
    if (rank == 0)
        std::vector<int> local_vec((ROWS_COUNT % size)*COLS_COUNT + delta);
    else
        std::vector<int> local_vec(delta);

    // receive these addresses in every process
    if (rank == 0) {
        local_vec = std::vector<int>(matrix.begin(), matrix.begin() +
        (ROWS_COUNT % size)*COLS_COUNT + delta);
    } else {
        MPI_Status status;
        MPI_Recv(&local_vec[0], delta,
                    MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    std::vector<int> local_mins;

    // in the cycle below divide local_vec into separate matrix rows
    // and return concrete mins of matrix rows into local_mins.
    // But we have to consider the case of 0 process, because there
    // can be remainder + delta
    if (rank == 0) {
        // remainder for 0 process
        for (int i = 0; i < ROWS_COUNT % size; i++) {
            local_mins.push_back(getSequentialMinInVec(std::vector<int>(
                local_vec.begin(), local_vec.begin() + (i+1) * COLS_COUNT )));
        }

        // delta for o process
        for (int i = 0; i < (ROWS_COUNT / size); i++) {
            local_mins.push_back(getSequentialMinInVec(std::vector<int>(
                local_vec.begin() + (ROWS_COUNT % size)*COLS_COUNT,
                local_vec.begin() + (ROWS_COUNT % size)*COLS_COUNT +
                (i+1) * COLS_COUNT )));
        }
    } else {  // another processes have only delta
        for (int i = 0; i < (ROWS_COUNT / size); i++) {
            local_mins.push_back(getSequentialMinInVec(std::vector<int>(
                local_vec.begin(), local_vec.begin() + (i+1) * COLS_COUNT )));
        }
    }

    std::vector<int> minsByParallel(ROWS_COUNT);

    if (rank == 0) {
        minsByParallel = local_mins;
        MPI_Status status;
        for (int proc = 1; proc < size; proc++) {
            std::vector<int> buffer(ROWS_COUNT / size);
            MPI_Recv(&buffer[0], ROWS_COUNT / size, MPI_INT,
                proc, 0, MPI_COMM_WORLD, &status);
            minsByParallel.insert(minsByParallel.end(), buffer.begin(),
                buffer.end());
        }
    } else {
        MPI_Send(&local_mins[0], ROWS_COUNT / size,
            MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    return minsByParallel;
}

void printMins() {
    for (int i = 0; i < ROWS_COUNT; i++) {
        std::cout << minsBySequential[i] << " ";
    }
    std::cout << std::endl;
}


void printMinsByParallel() {
    for (int i = 0; i < ROWS_COUNT; i++) {
        std::cout << minsByParallel[i] << " ";
    }
    std::cout << std::endl;
}
