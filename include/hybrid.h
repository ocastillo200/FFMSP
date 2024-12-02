#ifndef HYBRID_H
#define HYBRID_H

std::pair<int, std::string> hybridAlgorithm(
    int stringLength,
    const std::vector<char> &alphabet,
    const std::vector<std::string> &omega,
    double epsilon,
    double t,
    double timeLimit,
    int populationSize,
    int maxGenerations,
    double mutationRate,
    double crossoverRate,
    int tuning);

#endif