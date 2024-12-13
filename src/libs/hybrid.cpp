#include "grasp.h"
#include "greedy.h"
#include "utils.h"
#include "genetic.h"
#include "hybrid.h"
#include "algorithm"

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
    int tuning = 0)
{
    int threshold = static_cast<int>(t * stringLength);
    std::vector<Individual> population = initializePopulation(stringLength, alphabet, omega, epsilon, t, populationSize, tuning);
    Individual best = *std::max_element(population.begin(), population.end(),
                                        [](const Individual &a, const Individual &b)
                                        {
                                            return a.fitness < b.fitness;
                                        });
    if (tuning == 0)
    {
        std::cout << "Población inicial generada con Genético." << std::endl;
        std::cout << "Mejor solución inicial: " << best.genes << " (Fitness: " << best.fitness << ")" << std::endl;
    }
    auto start = std::chrono::high_resolution_clock::now();
    int iteration = 0;
    while (iteration < maxGenerations)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        double elapsedTime = std::chrono::duration<double>(currentTime - start).count();
        if (elapsedTime >= timeLimit)
            if (tuning == 0)
            {
                {
                    std::cout << "Límite de tiempo alcanzado. " << elapsedTime << std::endl;
                    break;
                }
                if (best.fitness == omega.size())
                {
                    std::cout << "Solución perfecta encontrada." << std::endl;
                    break;
                }
            }

        std::sort(population.begin(), population.end(),
                  [](const Individual &a, const Individual &b)
                  { return a.fitness > b.fitness; });
        int eliteCount = static_cast<int>(0.2 * populationSize);
        std::vector<Individual> newPopulation(population.begin(), population.begin() + eliteCount);
        while (newPopulation.size() < populationSize)
        {
            Individual parent1 = tournamentSelection(population, populationSize);
            Individual parent2 = tournamentSelection(population, populationSize);

            auto [child1, child2] = crossover(parent1, parent2, stringLength, crossoverRate);

            mutate(child1, alphabet, mutationRate);
            mutate(child2, alphabet, mutationRate);

            child1.fitness = calculateCost(child1.genes, omega, threshold, 0);
            child2.fitness = calculateCost(child2.genes, omega, threshold, 0);
            child1.isModified = true;
            child2.isModified = true;

            newPopulation.push_back(child1);
            if (newPopulation.size() < populationSize)
            {
                newPopulation.push_back(child2);
            }
        }
        population = newPopulation;

        for (int i = 0; i < eliteCount; ++i)
        {
            if (population[i].isModified)
            {
                int localSearchFitness;
                std::string localSearchSolution;
                std::tie(localSearchSolution, localSearchFitness) = localSearch(population[i].genes, population[i].fitness, omega, alphabet, t);
                if (localSearchFitness <= population[i].fitness)
                {
                    population[i].isModified = false;
                }
                population[i].fitness = localSearchFitness;
                population[i].genes = localSearchSolution;
            }
        }
        Individual bestIndividual = *max_element(population.begin(), population.end(),
                                                 [](const Individual &a, const Individual &b)
                                                 {
                                                     return a.fitness < b.fitness;
                                                 });
        if (bestIndividual.fitness > best.fitness)
        {
            best = bestIndividual;
            if (tuning == 0)
            {
                std::cout << "Nueva mejor solución encontrada (Generación: " << iteration + 1 << ")" << std::endl;
                std::cout << "Genes: " << best.genes << " (Fitness: " << best.fitness << ")" << std::endl;
            }
        }
        ++iteration;
    }
    std::cout << "Maximo de iteraciones alcanzadas." << std::endl;
    return {best.fitness, best.genes};
}
