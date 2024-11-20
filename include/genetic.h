#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <iostream>
#include <vector>
#include <random>
#include <string>

struct Individual
{
    std::string genes;
    int fitness;
    Individual(const std::string &genes) : genes(genes), fitness(0) {}
};

std::vector<Individual> initializePopulation(int stringLength, const std::vector<char> &alphabet, const std::vector<std::string> &omega, double epsilon, double t);
Individual tournamentSelection(const std::vector<Individual> &population);
std::pair<Individual, Individual> crossover(const Individual &parent1, const Individual &parent2, int stringLength);
void mutate(Individual &ind, const std::vector<char> &alphabet);
std::pair<int, std::string> geneticAlgorithm(int stringLength, const std::vector<char> &alphabet, const std::vector<std::string> &omega, double epsilon, double t, double timeLimit, int populationSize, int maxGenerations, double mutationRate, double crossoverRate, int tunning);

#endif
