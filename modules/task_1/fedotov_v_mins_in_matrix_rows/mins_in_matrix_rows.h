// Copyright 2019 Fedotov Vlad
#ifndef MODULES_MINS_IN_MATRIX_ROWS_H_
#define MODULES_MINS_IN_MATRIX_ROWS_H_

#include <vector>
#include <string>

/*
//#ifndef VARIABLES_H_
//#define VARIABLES_H_
const static int ROWS_COUNT = 10;
const static int COLS_COUNT = 10;

std::vector<int> matrix[ROWS_COUNT]; // matrix of ROWS_COUNT rows, that's represented as array of vectors<int>
int mins[ROWS_COUNT]; // array of mins of each row
//#endif // VARIABLES_H_
*/
std::vector<int> getRandomVector();
void getRandomMatrix();
void printMatrix();

int getSequentialMinInRow(std::vector<int> row); // in definite row, not in all matrix
int getParallelMinInRow(std::vector<int> row); // in definite row, not in all matrix

void getSequentialMinsInMatrix(); // mins of each row of matrix will be written in array mins[ROWS_COUNT]
void getParallelMinsInMatrix(); // mins of each row of matrix will be written in array mins[ROWS_COUNT]

#endif  // MODULES_MINS_IN_MATRIX_ROWS_H_
