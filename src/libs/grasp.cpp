#include <iostream>
#include <vector>
#include <random>
#include <functional>
#include <chrono>
#include "greedy.h"
#include <iomanip>

// Función de búsqueda local que mejora la solución
std::pair<std::string, int> localSearch(const std::string &initialSolution, int initialFitness, const std::vector<std::string> &inputStrings, const std::vector<char> &alphabet, double t, bool bestImprovement = true)
{
    std::string bestSolution = initialSolution;
    int bestFitness = initialFitness;
    bool improvementFound = true;
    while (improvementFound)
    {
        improvementFound = false;
        for (size_t i = 0; i < bestSolution.size(); ++i)
        {
            for (const char &letter : alphabet)
            {
                if (bestSolution[i] != letter)
                {
                    std::string newSolution = bestSolution;
                    newSolution[i] = letter;
                    int newFitness = calculateCost(newSolution, inputStrings, t, 0, newSolution.size());
                    if (newFitness > bestFitness)
                    {
                        return {newSolution, newFitness};
                    }
                }
            }
        }
    }
    return {bestSolution, bestFitness};
}

std::pair<std::string, int> GRASP(const std::vector<std::string> &inputStrings, const std::vector<char> &alphabet, int maxIterations, double epsilon, double t, double timelimit)
{
    int stringLength = inputStrings[0].size();
    std::string bestSolution;
    int bestFitness = -1;

    auto totalStart = std::chrono::high_resolution_clock::now(); // Tiempo total desde el inicio

    for (int iter = 0; iter < maxIterations; ++iter)
    {
        auto greedyResult = constructGreedySolution(stringLength, alphabet, inputStrings, epsilon, t);
        std::string currentSolution = greedyResult.second;
        int currentFitness = greedyResult.first;
        std::string localsearchSolution;
        int localsearchFitness;

        std::tie(localsearchSolution, localsearchFitness) = localSearch(currentSolution, currentFitness, inputStrings, alphabet, t);

        if (localsearchFitness > bestFitness)
        {
            bestSolution = localsearchSolution;
            bestFitness = localsearchFitness;
            auto solutionTime = std::chrono::high_resolution_clock::now();
            double elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(solutionTime - totalStart).count();
            std::cout
                << "Fitness: " << bestFitness << std::endl
                << "Time at which solution was found: " << elapsedTime << "s" << std::endl;
        }
        auto currentTime = std::chrono::high_resolution_clock::now();
        double totalElapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - totalStart).count();
        if (totalElapsedTime > timelimit || bestFitness == inputStrings.size())
        {
            break;
        }
    }

    return {bestSolution, bestFitness};
}