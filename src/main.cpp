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
    const string filename = readParam(argc, args, "i");
    const string tresholdP = readParam(argc, args, "th");
    const string epsilonP = readParam(argc, args, "e");
    const string timelimitP = readParam(argc, args, "t");

    if (filename.empty() || tresholdP.empty() || epsilonP.empty() || timelimitP.empty())
    {
        cerr << "Uso: " << argv[0] << "-i <instancia> -th <threshold> -e <epsilon> -t <tiempo límite>" << endl;
        return 1;
    }
    double threshold, epsilon, timelimit;
    try
    {
        threshold = stod(tresholdP);
        timelimit = stod(timelimitP);
        epsilon = stod(epsilonP);
        if (threshold < 0 || threshold > 1 || epsilon < 0 || epsilon > 1)
        {
            cerr << "El valor de threshold y el epsilon deben estar entre 0 y 1." << endl;
            return 1;
        }
        if (timelimit < 0)
        {
            cerr << "El valor de timelimit debe ser mayor a 0." << endl;
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


    chrono::high_resolution_clock::time_point start, end;
    start = chrono::high_resolution_clock::now();
    pair<int, string> solution = GRASP(omega, alphabet, 500, epsilon, threshold, timelimit, true);
    end = chrono::high_resolution_clock::now();

    double quality = solution.first / (double)omega.size() * 100;

    cout << endl << "Solución construida: " << solution.second << endl;
    cout << "Tiempo de ejecución: "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms" << endl;
    cout << "Calidad de la solución: " << quality << "% (" << (quality / 100) * omega.size() << " palabras)" << endl;
    return 0;
}
