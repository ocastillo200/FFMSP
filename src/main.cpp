#include <bits/stdc++.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "greedy.h"
#include "utils.h"
#include "cmsa.h"

using namespace std;

int main(int argc, char *argv[]) {
    vector<string> args(argv, argv + argc);
    const string filename = readParam(argc, args, "i");
    const string tresholdP = readParam(argc, args, "th");
    const string epsilonP = readParam(argc, args, "e");
    const string computeTimeP = readParam(argc, args, "t");
    if (filename.empty() || tresholdP.empty() || epsilonP.empty() || computeTimeP.empty()) {
        cerr << "Uso: " << argv[0] << "-i <instancia> -t <tiempo máximo> -th <threshold> -e <epsilon>" << endl;
        return 1;
    }
    double threshold, epsilon;
    int computeTime;

    try {
        threshold = stod(tresholdP);
        if (threshold < 0 || threshold > 1) {
            cerr << "El valor de threshold debe estar entre 0 y 1." << endl;
            return 1;
        }
        epsilon = stod(epsilonP);
        if (epsilon < 0 || epsilon > 1) {
            cerr << "El valor de epsilon debe estar entre 0 y 1." << endl;
            return 1;
        }
        computeTime = stoi(computeTimeP);
        if (computeTime < 0) {
            cerr << "El valor de tiempo de ejecución debe ser un número positivo." << endl;
            return 1;
        }
    } catch (const invalid_argument &e) {
        cerr << "Los valores de threshold, epsilon y el tiempo debe ser numérico." << endl;
        return 1;
    }
    int stringLength = getStringLengthFromFilename(filename);
    vector<char> alphabet = {'A', 'C', 'G', 'T'};
    vector<string> omega;
    ifstream infile(filename);
    string line;

    if (!infile) {
        cerr << "No se pudo abrir el archivo: " << filename << endl;
        return 1;
    }

    while (infile >> line) {
        omega.push_back(line);
    }
    infile.close();
    pair<int, string> solution;
    chrono::high_resolution_clock::time_point start, end;
    // if (mode == "-Greedy") {
    //     start = chrono::high_resolution_clock::now();
    // solution =
    //     constructGreedySolution(stringLength, alphabet, omega, 0.15, threshold);
    // cout << solution.first << " " << solution.second << endl;
    //     end = chrono::high_resolution_clock::now();
    // } else if (mode == "-GreedyA") {
    //     start = chrono::high_resolution_clock::now();
    //     solution = constructGreedySolution(stringLength, alphabet, omega, epsilon,
    //                                        threshold);
    //     end = chrono::high_resolution_clock::now();
    // } else {
    //     cerr << "Modo no válido. Usa -Greedy o -GreedyA." << endl;
    //     return 1;
    // }

    // double quality = solution.first / (double)omega.size() * 100;

    // cout << "Solución construida: " << solution.second << endl;
    // cout << "Tiempo de ejecución: "
    //      << chrono::duration_cast<chrono::milliseconds>(end - start).count()
    //      << " ms" << endl;
    // cout << "Calidad de la solución: " << quality << "% (" << (quality / 100) * omega.size() << " palabras)" << endl;
    Solution s = CMSA(omega, alphabet, 10, 3, 300, epsilon, threshold, computeTime);
    for(auto &c : s.components) {
        cout << c.letter;
    }
    return 0;
}
