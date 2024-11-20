#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <string>
#include "greedy.h"
#include "genetic.h"
#include "chrono"

using namespace std;

mt19937 rng(time(0));

vector<Individual> initializePopulation(int stringLength, const vector<char> &alphabet, const vector<string> &omega, double epsilon, double t, int populationSize)
{
    // cout << "Inicializando población..." << endl;
    vector<Individual> population;
    for (int i = 0; i < populationSize; ++i)
    {
        pair<int, string> greedySolution = constructGreedySolution(stringLength, alphabet, omega, epsilon, t);
        Individual ind(greedySolution.second);
        ind.fitness = greedySolution.first;
        population.push_back(ind);
    }
    // cout << "Población inicializada." << endl;
    return population;
}

Individual tournamentSelection(const vector<Individual> &population, int populationSize)
{
    int tournamentSize = 3;
    Individual best = population[rng() % populationSize];
    for (int i = 1; i < tournamentSize; ++i)
    {
        Individual competitor = population[rng() % populationSize];
        if (competitor.fitness > best.fitness)
        {
            best = competitor;
        }
    }
    return best;
}

pair<Individual, Individual> crossover(const Individual &parent1, const Individual &parent2, int stringLength, double crossoverRate)
{
    Individual child1 = parent1;
    Individual child2 = parent2;

    if (uniform_real_distribution<>(0, 1)(rng) < crossoverRate)
    {
        int crossoverPoint = rng() % stringLength;
        for (int i = crossoverPoint; i < stringLength; ++i)
        {
            swap(child1.genes[i], child2.genes[i]);
        }
    }
    return {child1, child2};
}

void mutate(Individual &ind, const vector<char> &alphabet, double mutationRate)
{
    for (char &gene : ind.genes)
    {
        if (uniform_real_distribution<>(0, 1)(rng) < mutationRate)
        {
            gene = alphabet[rng() % alphabet.size()];
        }
    }
}

pair<int, string> geneticAlgorithm(int stringLength, const vector<char> &alphabet, const vector<string> &omega, double epsilon, double t, double timeLimit, int populationSize, int maxGenerations, double mutationRate, double crossoverRate)
{
    vector<Individual> population = initializePopulation(stringLength, alphabet, omega, epsilon, t, populationSize);
    Individual best = *max_element(population.begin(), population.end(),
                                   [](const Individual &a, const Individual &b)
                                   {
                                       return a.fitness < b.fitness;
                                   });

    auto start = chrono::high_resolution_clock::now();
    int eliteCount = static_cast<int>(0.1 * populationSize);

    for (int generation = 0; generation < maxGenerations; ++generation)
    {
        if (chrono::duration<double>(chrono::high_resolution_clock::now() - start).count() > timeLimit)
        {
            break;
        }

        vector<Individual> newPopulation;
        sort(population.begin(), population.end(), [](const Individual &a, const Individual &b)
             { return a.fitness > b.fitness; });
        newPopulation.insert(newPopulation.end(), population.begin(), population.begin() + eliteCount);

        while (newPopulation.size() < populationSize)
        {
            Individual parent1 = tournamentSelection(population, populationSize);
            Individual parent2 = tournamentSelection(population, populationSize);

            auto [child1, child2] = crossover(parent1, parent2, stringLength, crossoverRate);

            mutate(child1, alphabet, mutationRate);
            mutate(child2, alphabet, mutationRate);

            child1.fitness = calculateCost(child1.genes, omega, t, 0, child1.genes.size());
            child2.fitness = calculateCost(child2.genes, omega, t, 0, child2.genes.size());

            newPopulation.push_back(child1);
            if (newPopulation.size() < populationSize)
            {
                newPopulation.push_back(child2);
            }
        }

        population = newPopulation;

        Individual bestIndividual = *max_element(population.begin(), population.end(),
                                                 [](const Individual &a, const Individual &b)
                                                 {
                                                     return a.fitness < b.fitness;
                                                 });
        // cout << "Generación " << generation + 1 << ": Mejor Aptitud = " << bestIndividual.fitness
        //      << " | Mejor Cadena = " << bestIndividual.genes << endl;

        if (bestIndividual.fitness > best.fitness)
        {
            best = bestIndividual;
        }
    }
    return {best.fitness, best.genes};
}
