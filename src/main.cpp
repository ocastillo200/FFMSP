#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "greedy.h"
#include <bits/stdc++.h>

using namespace std;

int getStringLengthFromFilename(const string &filename)
{
    size_t firstDash = filename.find('-');
    size_t secondDash = filename.find('-', firstDash + 1);
    string stringLengthStr = filename.substr(firstDash + 1, secondDash - firstDash - 1);
    return stoi(stringLengthStr);
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cerr << "Uso: " << argv[0] << " [-Greedy | -GreedyA] -i <instancia-problema> -th <threshold> -e <valor epsilon para greedy aleatorizado>" << endl;
        return 1;
    }
    string mode = argv[1];
    string filename;
    double threshold;
    double epsilon;
    string instanceFolder = "../instancias/";

    for (int i = 2; i < argc; ++i)
    {
        if (string(argv[i]) == "-i")
        {
            filename = instanceFolder + argv[++i];
        }
        else if (string(argv[i]) == "-th")
        {
            threshold = stod(argv[++i]);
        }
        else if (string(argv[i]) == "-e" && mode == "-GreedyA")
        {
            epsilon = stod(argv[++i]);
        }
        else
        {
            cerr << "Argumento no válido: " << argv[i] << endl;
            return 1;
        }
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

    while (getline(infile, line))
    {
        omega.push_back(line);
    }
    infile.close();

    pair<int, string> solution;

    if (mode == "-Greedy")
    {
        solution = constructGreedySolution(stringLength, alphabet, omega, 1, threshold);
    }
    else if (mode == "-GreedyA")
    {
        solution = constructGreedySolution(stringLength, alphabet, omega, epsilon, threshold);
    }
    else
    {
        cerr << "Modo no válido. Usa -Greedy o -GreedyA." << endl;
        return 1;
    }

    double quality = solution.first / (double)omega.size() * 100;

    cout << "Solución construida: " << solution.second << endl;
    cout << "Calidad de la solución: " << quality << "%" << endl;
    return 0;
}
