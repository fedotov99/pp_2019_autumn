// Copyright 2019 Fedotov Vlad
#ifndef MODULES_TASK_2_FEDOTOV_V_DINING_PHILOSOPHERS_H_
#define MODULES_TASK_2_FEDOTOV_V_DINING_PHILOSOPHERS_H_

#include <vector>
#include <string>

void philosopher(int);
std::vector<bool> waiter(int);
void printVector(std::vector<bool>&);
void printVector(std::vector<int>&);

#endif  // MODULES_TASK_2_FEDOTOV_V_DINING_PHILOSOPHERS_H_