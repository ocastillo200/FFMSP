#include <iostream>
#include <fstream>
#include <map>
#include <bits/stdc++.h>
#include <sys/stat.h>

#include "utils.h"
#include "greedy.h"
#include "grasp.h"

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
    if (folder.empty() || mode.empty() || tresholdP.empty() || amountOfStrings.empty() || ((mode == "greedyA" || mode == "grasp") && epsilonP.empty()) || (mode == "grasp" && timeLimitP.empty())) {
        cerr << "Uso: " << argv[0] << "-mode <greedy | greedyA | grasp> -f <carpeta> -th <threshold> -e <epsilon (para greedy aleatorizado o grasp)> -n <cantidad de palabras> -t <tiempo límite (para grasp)>" << endl;
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
    double threshold, epsilon, timeLimit;
    int amountOfStringsInt;
    try {
        amountOfStringsInt = stoi(amountOfStrings);
        threshold = stod(tresholdP);
        if (threshold < 0 || threshold > 1) {
            cerr << "El valor de threshold debe estar entre 0 y 1." << endl;
            return 1;
        }
        if (mode == "grasp") {
            timeLimit = stod(timeLimitP);
            if (timeLimit <= 0) {
                cerr << "El valor de tiempo límite debe ser mayor a 0." << endl;
                return 1;
            }
        }
        if (mode == "greedyA" || mode == "grasp") {
            epsilon = stod(epsilonP);
            if (epsilon < 0 || epsilon > 1) {
                cerr << "El valor de epsilon debe estar entre 0 y 1." << endl;
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
        pair<int, string> solution = (mode == "greedy" || mode == "greedyA") ?
                                     constructGreedySolution(amountOfStringsInt, alphabet, omega, mode == "greedy" ? 1 : epsilon, threshold) :
                                     GRASP(omega, alphabet, 500, epsilon, threshold, timeLimit, false);
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
        if (mode == "greedyA" || mode == "grasp") {
            double standardDeviation = calculateStandardDeviation(solution.second, mean);
            cout << "Desviación estándar: " << standardDeviation << "% (" << standardDeviation * (amountOfStringsInt / 100) << " palabras)" << endl;
        }
        cout << "Tiempo de ejecución: " << times[solution.first] << " ms" << endl;
        cout << "Tiempo de ejecución promedio: " << times[solution.first] / amountOfStringsInt << " ms" << endl;
    }
    return 0;
}