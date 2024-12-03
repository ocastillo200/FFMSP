#include <iostream>
#include <fstream>
#include <map>
#include <bits/stdc++.h>
#include <sys/stat.h>

#include "utils.h"
#include "greedy.h"
#include "grasp.h"
#include "genetic.h"
#include "hybrid.h"

using namespace std;

bool isDirectory(const string &path) {
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0) {
        return false;
    }
    return S_ISDIR(statbuf.st_mode);
}

int main(int argc, char *argv[]) {
    vector<string> args(argv, argv + argc);
    const string folder = readParam(argc, args, "f");
    const string mode = readParam(argc, args, "mode");
    const string tresholdP = readParam(argc, args, "th");
    const string epsilonP = readParam(argc, args, "e");
    const string amountOfStrings = readParam(argc, args, "n");
    const string timeLimitP = readParam(argc, args, "t");
    const string populationSizeP = readParam(argc, args, "p");
    const string maxGenerationsP = readParam(argc, args, "g");
    const string mutationRateP = readParam(argc, args, "m");
    const string crossoverRateP = readParam(argc, args, "c");
    const string tunningP = readParam(argc, args, "tn");
    if (folder.empty() || mode.empty() || tresholdP.empty() || amountOfStrings.empty() || ((mode == "greedyA" || mode == "grasp") && epsilonP.empty()) || (mode == "grasp" && timeLimitP.empty())) {
        cerr << "Uso: " << argv[0] << "-mode <greedy | greedyA | grasp | genetic> -f <carpeta> -th <threshold> -e <epsilon (para greedy aleatorizado, grasp o genetico)> -n <cantidad de palabras> -t <tiempo límite (para grasp o genetico)> -p <tamaño población, (genetico)> -g <cantidad de generaciones, (genetico)> -m <probabilidad de mutación, (genetico)> -c <probabilidad de crossover, (genetico)> -tn <modo tunning>, (genetico)" << endl;
        return 1;
    }
    if (!isDirectory(folder)) {
        cerr << "El directorio '" << folder << "' no existe o no es valido." << endl;
        return 1;
    }
    vector<pair<string, string>> files = getFilesFromFolder(folder, amountOfStrings);
    if (files.empty()) {
        cerr << "No se encontraron archivos en el directorio con la cantidad de palabras especificada." << endl;
        return 1;
    }
    // for (const pair<string, string> &file : files) {
    //     cout << file.first << " - " << file.second << endl;
    // }
    // return 0;
    double threshold, epsilon, timeLimit, mutationRate, crossoverRate;
    int amountOfStringsInt, populationSize, maxGenerations, tunning = 0;
    try {
        amountOfStringsInt = stoi(amountOfStrings);
        threshold = stod(tresholdP);
        if(tunningP != "") {
            tunning = stoi(tunningP);
        }
        if (threshold < 0 || threshold > 1) {
            cerr << "El valor de threshold debe estar entre 0 y 1." << endl;
            return 1;
        }
        if (mode == "grasp" || mode == "genetic") {
            timeLimit = stod(timeLimitP);
            if (timeLimit <= 0) {
                cerr << "El valor de tiempo límite debe ser mayor a 0." << endl;
                return 1;
            }
        }
        if (mode == "greedyA" || mode == "grasp" || mode == "genetic") {
            epsilon = stod(epsilonP);
            if (epsilon < 0 || epsilon > 1) {
                cerr << "El valor de epsilon debe estar entre 0 y 1." << endl;
                return 1;
            }
        }
        if (mode == "genetic" || mode == "hybrid") {
            populationSize = stoi(populationSizeP);
            maxGenerations = stoi(maxGenerationsP);
            mutationRate = stod(mutationRateP);
            crossoverRate = stod(crossoverRateP);
            if (populationSize <= 0 || maxGenerations <= 0) {
                cerr << "El tamaño de la población y la cantidad de generaciones deben ser mayores a 0." << endl;
                return 1;
            }
            if (mutationRate < 0 || mutationRate > 1 || crossoverRate < 0 || crossoverRate > 1) {
                cerr << "La probabilidad de mutación y crossover deben estar entre 0 y 1." << endl;
                return 1;
            }
        }
    } catch (const invalid_argument &e) {
        cout << tresholdP << " | " << epsilonP << endl;
        cerr << "Los valores de threshold, epsilon y n deben ser numéricos." << endl;
        return 1;
    }
    const vector<char> alphabet = {'A', 'C', 'G', 'T'};
    map<string, vector<double>> filesSolutions;
    map<string, double> times;
    for (const pair<string, string> &file : files) {
        vector<string> omega;
        ifstream infile(file.second);
        string line;
        while (infile >> line) {
            omega.push_back(line);
        }
        infile.close();
        auto startTime = chrono::high_resolution_clock::now();
        // pair<int, string> solution = (mode == "greedy" || mode == "greedyA") ?
        //                              constructGreedySolution(amountOfStringsInt, alphabet, omega, mode == "greedy" ? 1 : epsilon, threshold) :
        //                              GRASP(omega, alphabet, 500, epsilon, threshold, timeLimit, false);
        // pair<int, string> solution = geneticAlgorithm(amountOfStringsInt, alphabet, omega, epsilon, threshold, timeLimit, populationSize, maxGenerations, mutationRate, crossoverRate, tunning);
        pair<int, string> solution = hybridAlgorithm(amountOfStringsInt, alphabet, omega, epsilon, threshold, timeLimit, populationSize, maxGenerations, mutationRate, crossoverRate, tunning);
        auto endTime = chrono::high_resolution_clock::now();
        double quality = solution.first / (double)omega.size() * 100;
        filesSolutions[file.first].push_back(quality);
        times[file.first] += chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
    }
    cout << "Ejecución de " << files.size() << " archivos con " << amountOfStringsInt << " palabras, con un threshold de " << threshold << (mode == "greedyA" || mode == "grasp" ? " y epsilon de " + epsilonP : "") << endl;
    cout << "Resultados:" << endl;
    for (const auto &solution : filesSolutions) {
        cout << endl << "Cantidad de caracteres: " << solution.first << endl;
        double mean = accumulate(solution.second.begin(), solution.second.end(), 0.0) / solution.second.size();
        cout << "Media: " << mean << "% (" <<  mean * (amountOfStringsInt / 100) << " palabras)" << endl;
        if (mode == "greedyA" || mode == "grasp" || mode == "genetic") {
            double standardDeviation = calculateStandardDeviation(solution.second, mean);
            cout << "Desviación estándar: " << standardDeviation << "% (" << standardDeviation * (amountOfStringsInt / 100) << " palabras)" << endl;
        }
        cout << "Tiempo de ejecución: " << times[solution.first] << " ms" << endl;
        cout << "Tiempo de ejecución promedio: " << times[solution.first] / amountOfStringsInt << " ms" << endl;
    }
    return 0;
}