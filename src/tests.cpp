#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>
#include <bits/stdc++.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include "utils.h"
#include "greedy.h"

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
    const string mode =
        containsFlag(argc, args, "GreedyA") ? "-GreedyA" : "-Greedy";
    const string tresholdP = readParam(argc, args, "th");
    const string epsilonP = readParam(argc, args, "e");
    const string amountOfStrings = readParam(argc, args, "n");
    if (folder.empty() || mode.empty() || tresholdP.empty() || amountOfStrings.empty() || (mode == "-GreedyA" && epsilonP.empty())) {
        cerr << "Uso: " << argv[0] << " [-Greedy | -GreedyA] -f <carpeta> -th <threshold> -e <epsilon (para greedy aleatorizado)> -n <cantidad de palabras>" << endl;
        return 1;
    }
    if (isDirectory(folder)) {
        cerr << "El directorio '" << folder << "' no existe o no es valido." << endl;
        return 1;
    }
    vector<pair<string, string>> files = getFilesFromFolder(folder, amountOfStrings);
    if (files.empty()) {
        cerr << "No se encontraron archivos en el directorio con la cantidad de palabras especificada." << endl;
        return 1;
    }
    double threshold, epsilon;
    int amountOfStringsInt;
    try {
        amountOfStringsInt = stoi(amountOfStrings);
        threshold = stod(tresholdP);
        if (threshold < 0 || threshold > 1) {
            cerr << "El valor de threshold debe estar entre 0 y 1." << endl;
            return 1;
        }
        if (mode == "-GreedyA") {
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
        while (getline(infile, line)) {
            omega.push_back(line);
        }
        infile.close();
        auto startTime = chrono::high_resolution_clock::now();
        pair<int, string> solution = constructGreedySolution(amountOfStringsInt, alphabet, omega, mode == "-GreedyA" ? epsilon : 1, threshold);
        auto endTime = chrono::high_resolution_clock::now();
        double quality = solution.first / (double)omega.size() * 100;
        filesSolutions[file.first].push_back(quality);
        times[file.first] += chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
    }
    cout << "Ejecución de " << files.size() << " archivos con " << amountOfStringsInt << " palabras, con un threshold de " << threshold << (mode == "-GreedyA" ? " y epsilon de " + epsilonP : "") << endl;
    cout << "Resultados:" << endl;
    for (const auto &solution : filesSolutions) {
        cout << endl << "Cantidad de caracteres: " << solution.first << endl;
        double mean = accumulate(solution.second.begin(), solution.second.end(), 0.0) / solution.second.size();
        cout << "Media: " << mean << "% (" <<  mean * (amountOfStringsInt / 100) << " palabras)" << endl;
        if (mode == "-GreedyA") {
            double standardDeviation = calculateStandardDeviation(solution.second, mean);
            cout << "Desviación estándar: " << standardDeviation << "% (" << standardDeviation * (amountOfStringsInt / 100) << " palabras)" << endl;
        }
        cout << "Tiempo de ejecución: " << times[solution.first] << " ms" << endl;
        cout << "Tiempo de ejecución promedio: " << times[solution.first] / amountOfStringsInt << " ms" << endl;
    }
    return 0;
}