// Copyright 2019 Fedotov Vlad
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <ctime>
#include <iostream>
#include "../../../modules/task_1/fedotov_v_mins_in_matrix_rows/mins_in_matrix_rows.h"

std::vector<int> getRandomMatrix(int m, int n) {
    std::vector<int> matrix(m * n);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < m * n; i++) {
        matrix[i] = gen() % 100;
    }

    return matrix;
}

int getSequentialMinInVec(const std::vector<int>& vector) {
    int min = vector[0];
    for (unsigned int i = 0; i < vector.size(); i++) {
        min = std::min(min, vector[i]);
    }
    return min;
}

std::vector<int> getSequentialMinsInMatrix(const std::vector<int>& matr,
                                           int m, int n) {
    std::vector<int> minsBySequential(m);
    for (int i = 0; i < m; i++) {
        minsBySequential[i] = getSequentialMinInVec(std::vector<int>(
            matr.begin() + i * n, matr.begin() + i * n + n));
    }
    return minsBySequential;
}

std::vector<int> getParallelMinsInMatrix(const std::vector<int>& matr,
                                         int m, int n) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int delta = (m / size) * n;  // number of elements per process

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(&matr[0] + proc * delta, delta,
                        MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> local_vec(delta);
    if (rank == 0) {
        local_vec = std::vector<int>(matr.begin(), matr.begin() + delta);
    } else {
        MPI_Status status;
        MPI_Recv(&local_vec[0], delta,
                    MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    std::vector<int> local_mins(0);  // m/size

    if (rank == 0) {
        for (int i = 0; i < (m / size); i++) {
            local_mins.push_back(getSequentialMinInVec(std::vector<int>(
                local_vec.begin() + i * n,
                local_vec.begin() + (i+1) * n)));
        }
    } else {
        for (int i = 0; i < (m / size); i++) {
            local_mins.push_back(getSequentialMinInVec(std::vector<int>(
                local_vec.begin() + i * n, local_vec.begin() + (i+1) * n)));
        }
    }

    std::vector<int> minsByParallel(0);  // m
    if (rank == 0) {
        minsByParallel.insert(minsByParallel.begin(), local_mins.begin(),
            local_mins.end());
        MPI_Status status;
        for (int proc = 1; proc < size; proc++) {
            std::vector<int> buffer(m / size);
            MPI_Recv(&buffer[0], m / size, MPI_INT,
                proc, 0, MPI_COMM_WORLD, &status);
            minsByParallel.insert(minsByParallel.end(), buffer.begin(),
                buffer.end());
        }
    } else {
        MPI_Send(&local_mins[0], m / size,
            MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    return minsByParallel;
}

void printVector(const std::vector<int>& vec) {
    for (std::vector<int>::const_iterator it = vec.begin();
        it < vec.end(); it++)
        std::cout << *it << " ";
    std::cout << "Vector printed " << std::endl;
}

void printMatrix(const std::vector<int>& matr, int m, int n) {
    for (int i = 1; i <= m * n; i++) {
            std::cout << matr[i-1] << " ";
            if (i % n == 0)  // move to new row
                std::cout << std::endl;
        }
}

/*
void getParallelMinsInMatrix() {
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
    // and receive these addresses in every process
    std::vector<int> local_vec((ROWS_COUNT % size)*COLS_COUNT + delta);
    if (rank == 0) {
        local_vec = std::vector<int>(matrix.begin(), matrix.begin() +
        (ROWS_COUNT % size)*COLS_COUNT + delta);
    } else {
        MPI_Status status;
        MPI_Recv(&local_vec[0], delta,
                    MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    // in the cycle below divide local_vec into separate matrix rows
    // and return concrete mins of matrix rows into local_mins.
    // But we have to consider the case of 0 process, because there
    // can be remainder + delta
    std::vector<int> local_mins(ROWS_COUNT % size + ROWS_COUNT / size);
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
        local_mins = std::vector<int>(ROWS_COUNT / size);
        for (int i = 0; i < (ROWS_COUNT / size); i++) {
            local_mins.push_back(getSequentialMinInVec(std::vector<int>(
                local_vec.begin(), local_vec.begin() + (i+1) * COLS_COUNT )));
        }
    }

    // std::vector<int> minsByParallel(ROWS_COUNT);
    if (rank == 0) {
        minsByParallel = local_mins;
        MPI_Status status;
        for (int proc = 1; proc < size; proc++) {
            std::vector<int> buffer(ROWS_COUNT / size);
            MPI_Recv(&buffer[0], ROWS_COUNT / size, MPI_INT,
                proc, 0, MPI_COMM_WORLD, &status);
            minsByParallel.insert(minsByParallel.begin(), buffer.begin(),
                buffer.end());
        }
    } else {
        MPI_Send(&local_mins[0], ROWS_COUNT / size,
            MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
}
*/
