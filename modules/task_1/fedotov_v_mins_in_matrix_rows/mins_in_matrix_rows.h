// Copyright 2019 Fedotov Vlad
#ifndef MODULES_MINS_IN_MATRIX_ROWS_H_
#define MODULES_MINS_IN_MATRIX_ROWS_H_

#include <vector>
#include <string>

std::vector<int> getRandomVector();
void getRandomMatrix();
void printVector(std::vector<int> vec);
void printMatrix();

int getSequentialMinInRow(std::vector<int> row); // in definite row, not in all matrix
int getParallelMinInRow(std::vector<int> row); // in definite row, not in all matrix

void getSequentialMinsInMatrix(); // mins of each row of matrix will be written in array mins[ROWS_COUNT]
void getParallelMinsInMatrix(); // mins of each row of matrix will be written in array mins[ROWS_COUNT]

#endif  // MODULES_MINS_IN_MATRIX_ROWS_H_
