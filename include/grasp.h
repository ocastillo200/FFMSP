#ifndef GRASP_H
#define GRASP_H

#include <iostream>
#include <vector>
#include <random>
#include <functional>
#include <chrono>

std::pair<std::string, int> localSearch(const std::string &initialSolution, int initialFitness, const std::vector<std::string> &inputStrings, const std::vector<char> &alphabet, double t);

std::pair<std::string, int> GRASP(const std::vector<std::string> &inputStrings, const std::vector<char> &alphabet, int maxIterations, double epsilon, double t, double timelimit);

#endif