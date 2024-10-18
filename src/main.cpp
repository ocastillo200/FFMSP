#include <bits/stdc++.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include "grasp.h"
#include "greedy.h"
#include "utils.h"

using namespace std;

int main(int argc, char *argv[])
{
    vector<string> args(argv, argv + argc);
    const string mode =
        containsFlag(argc, args, "GreedyA") ? "-GreedyA" : "-Greedy";
    const string filename = readParam(argc, args, "i");
    const string tresholdP = readParam(argc, args, "th");
    const string epsilonP = readParam(argc, args, "e");
    const string timelimitP = readParam(argc, args, "t");

    if (filename.empty() || tresholdP.empty() || (mode == "-GreedyA" && epsilonP.empty()))
    {
        cerr << "Uso: " << argv[0] << " [-Greedy | -GreedyA] -i <instancia> -th <threshold> -e <epsilon (para greedy aleatorizado)>" << endl;
        return 1;
    }
    double threshold, epsilon, timelimit;
    try
    {
        threshold = stod(tresholdP);
        timelimit = stod(timelimitP);
        if (threshold < 0 || threshold > 1)
        {
            cerr << "El valor de threshold debe estar entre 0 y 1." << endl;
            return 1;
        }

        epsilon = stod(epsilonP);
        if (epsilon < 0 || epsilon > 1)
        {
            cerr << "El valor de epsilon debe estar entre 0 y 1." << endl;
            return 1;
        }
    }
    catch (const invalid_argument &e)
    {
        cerr << "Los valores de threshold y epsilon deben ser numéricos." << endl;
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

    pair<string, int> solution = GRASP(omega, alphabet, 500, epsilon, threshold, timelimit);

    // chrono::high_resolution_clock::time_point start, end;
    // if (mode == "-Greedy") {
    //     start = chrono::high_resolution_clock::now();
    //     solution =
    //         constructGreedySolution(stringLength, alphabet, omega, 1, threshold);
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
    return 0;
}
