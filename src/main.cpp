#include <bits/stdc++.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include "grasp.h"
#include "greedy.h"
#include "utils.h"
#include "genetic.h"

using namespace std;

int main(int argc, char *argv[])
{
    vector<string> args(argv, argv + argc);
    const string filename = readParam(argc, args, "i");
    const string tresholdP = readParam(argc, args, "th");
    const string epsilonP = readParam(argc, args, "e");
    const string timelimitP = readParam(argc, args, "t");
    const string populationSizeP = readParam(argc, args, "p");
    const string maxGenerationsP = readParam(argc, args, "g");
    const string mutationRateP = readParam(argc, args, "m");
    const string crossoverRateP = readParam(argc, args, "c");

    if (filename.empty() || tresholdP.empty() || epsilonP.empty() || timelimitP.empty() || populationSizeP.empty() || maxGenerationsP.empty() || mutationRateP.empty() || crossoverRateP.empty())
    {
        cerr << "Uso: " << argv[0] << "-i <instancia> -th <threshold> -e <epsilon> -t <tiempo límite> -p <tamaño población> -g <cantidad de generaciones> -m <probabilidad de mutación> -c <probabilidad de crossover> -tn <modo tunning>" << endl;
        return 1;
    }
    int populationSize, maxGenerations, tunning;
    double threshold, epsilon, timelimit, mutationRate, crossoverRate;
    try
    {
        tunning = stoi(readParam(argc, args, "tn"));
        threshold = stod(tresholdP);
        timelimit = stod(timelimitP);
        epsilon = stod(epsilonP);
        populationSize = stoi(populationSizeP);
        maxGenerations = stoi(maxGenerationsP);
        mutationRate = stod(mutationRateP);
        crossoverRate = stod(crossoverRateP);
        if (threshold < 0 || threshold > 1 || epsilon < 0 || epsilon > 1)
        {
            cerr << "El valor de threshold y el epsilon deben estar entre 0 y 1." << endl;
            return 1;
        }
        if (populationSize < 0 || maxGenerations < 0)
        {
            cerr << "El tamaño de la población y la cantidad de generacionesdeben ser mayores a 0." << endl;
            return 1;
        }
        if (mutationRate < 0 || mutationRate > 1 || crossoverRate < 0 || crossoverRate > 1)
        {
            cerr << "La probabilidad de mutación y crossover deben estar entre 0 y 1." << endl;
            return 1;
        }
        if (timelimit < 0)
        {
            cerr << "El valor de timelimit debe ser mayor a 0." << endl;
            return 1;
        }
        if (tunning != 0 && tunning != 1)
        {
            cerr << "El valor de tunning debe ser 0 o 1." << endl;
            return 1;
        }
    }
    catch (const invalid_argument &e)
    {
        cerr << "Los valores de threshold, epsilon y otros para alg genético deben ser numéricos." << endl;
        return 1;
    }
    int stringLength = getStringLengthFromFilename(filename);
    vector<char> alphabet = {'A', 'C', 'G', 'T'};
    vector<string> omega;
    ifstream infile(filename);
    string line;

    if (!infile)
    {
        cerr << "No se pudo abrir el archivo: " << filename << endl;
        return 1;
    }

    while (infile >> line)
    {
        omega.push_back(line);
    }
    infile.close();

    chrono::high_resolution_clock::time_point start, end;
    start = chrono::high_resolution_clock::now();
    // pair<int, string> solution = GRASP(omega, alphabet, 500, epsilon, threshold, timelimit, true);
    pair<int, string> solutionGenetic = geneticAlgorithm(stringLength, alphabet, omega, epsilon, threshold, timelimit, populationSize, maxGenerations, mutationRate, crossoverRate);

    end = chrono::high_resolution_clock::now();

    int quality = solutionGenetic.first;
    if (tunning == 1)
    {
        cout << quality;
    }
    else
    {
        cout << "Calidad de la solución: " << quality << endl;
        cout << "Solución: " << solutionGenetic.second << endl;
        cout << "Tiempo de ejecución: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
    }
    return 0;
}
