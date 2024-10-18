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
        std::string currentBestSolution = bestSolution;
        int currentBestFitness = bestFitness;

        for (size_t i = 0; i < bestSolution.size(); ++i)
        {
            for (const char &letter : alphabet)
            {
                if (bestSolution[i] != letter)
                {
                    std::string newSolution = bestSolution;
                    newSolution[i] = letter;

                    int newFitness = calculateCost(newSolution, inputStrings, t, 0, newSolution.size());

                    // Si encontramos una mejora, aplicamos bestImprovement o firstImprovement
                    if (newFitness > currentBestFitness)
                    {
                        currentBestSolution = newSolution;
                        currentBestFitness = newFitness;
                        improvementFound = true;

                        if (!bestImprovement)
                        {
                            // Si es first improvement, paramos inmediatamente
                            bestSolution = currentBestSolution;
                            bestFitness = currentBestFitness;
                            return {bestSolution, bestFitness};
                        }
                    }
                }
            }
        }

        if (improvementFound)
        {
            bestSolution = currentBestSolution;
            bestFitness = currentBestFitness;
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
        // Iniciar construcción y búsqueda local
        auto greedyResult = constructGreedySolution(stringLength, alphabet, inputStrings, epsilon, t);
        std::string currentSolution = greedyResult.second;
        int currentFitness = greedyResult.first;

        std::tie(currentSolution, currentFitness) = localSearch(currentSolution, currentFitness, inputStrings, alphabet, t);

        // Si encontramos una mejor solución, la actualizamos y calculamos el tiempo transcurrido
        if (currentFitness > bestFitness)
        {
            bestSolution = currentSolution;
            bestFitness = currentFitness;

            // Calcular el tiempo desde el inicio hasta que se encontró esta solución
            auto solutionTime = std::chrono::high_resolution_clock::now();
            double elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(solutionTime - totalStart).count();

            // Imprimir la solución y el tiempo en que fue encontrada
            std::cout << "Solution found: " << bestSolution << std::endl
                      << "Fitness: " << bestFitness << std::endl
                      << "Time at which solution was found: " << elapsedTime << "s" << std::endl;
        }

        // Verificar el tiempo total transcurrido y el límite de tiempo
        auto currentTime = std::chrono::high_resolution_clock::now();
        double totalElapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - totalStart).count();
        if (totalElapsedTime > timelimit || bestFitness == inputStrings.size())
        {
            break;
        }
    }

    return {bestSolution, bestFitness};
}