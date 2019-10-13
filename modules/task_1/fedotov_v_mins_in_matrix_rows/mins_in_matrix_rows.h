// Copyright 2019 Fedotov Vlad
#ifndef MODULES_TASK_1_FEDOTOV_V_MINS_IN_MATRIX_ROWS_MINS_IN_MATRIX_ROWS_H_
#define MODULES_TASK_1_FEDOTOV_V_MINS_IN_MATRIX_ROWS_MINS_IN_MATRIX_ROWS_H_

#include <vector>
#include <string>

std::vector<int> getRandomMatrix(int m, int n);
int getSequentialMinInVec(const std::vector<int>& vector);
std::vector<int> getSequentialMinsInMatrix(const std::vector<int>& matr, int m, int n);
std::vector<int> getParallelMinsInMatrix(const std::vector<int>& matr, int m, int n);

void printVector(std::vector<int>& vec);
void printMatrix(std::vector<int>& matr, int m, int n);

#endif  // MODULES_TASK_1_FEDOTOV_V_MINS_IN_MATRIX_ROWS_MINS_IN_MATRIX_ROWS_H_
