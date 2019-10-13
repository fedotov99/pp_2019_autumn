// Copyright 2019 Fedotov Vlad
#ifndef MODULES_TASK_1_FEDOTOV_V_MINS_IN_MATRIX_ROWS_MINS_IN_MATRIX_ROWS_H_
#define MODULES_TASK_1_FEDOTOV_V_MINS_IN_MATRIX_ROWS_MINS_IN_MATRIX_ROWS_H_

#include <vector>
#include <string>

extern const int ROWS_COUNT;
extern const int COLS_COUNT;

// extern std::vector<int> matrix[];
extern std::vector<int> matrix;  // version 2 of realization
extern bool matrixInitialized;
extern int minsBySequential[];
// extern int minsByParallel[];

// std::vector<int> getRandomVector(int n = 0);  // if we don't use "n", vectors of matrix will be equal
void getRandomMatrix();

// int getSequentialMinInRow(std::vector<int> row);  // in definite row, not in all matrix
// int getParallelMinInRow(std::vector<int> row);  // in definite row, not in all matrix

int getSequentialMinInRow(int m);  // in definite row, not in all matrix
int getSequentialMinInVec(std::vector<int> vector);
// int getParallelMinInRow(int m);  // in definite row, not in all matrix

void getSequentialMinsInMatrix();  // mins of each row of matrix will be written in array mins[ROWS_COUNT]
std::vector<int> getParallelMinsInMatrix();  // mins of each row of matrix will be written in array mins[ROWS_COUNT]

void printVector(std::vector<int> vec);
void printMatrix();
void printMins();
// void printMinsByParallel();

#endif  // MODULES_TASK_1_FEDOTOV_V_MINS_IN_MATRIX_ROWS_MINS_IN_MATRIX_ROWS_H_
