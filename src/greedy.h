#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <bits/stdc++.h>

using namespace std;

// Función para calcular la distancia de Hamming entre dos cadenas de igual longitud
int hammingDistance(const string &s1, const string &s2);

// Función de evaluación para calcular cuántas cadenas cumplen con el umbral t, ignorando las que ya lo cumplen
int calculateCost(const string &currentSolution, const vector<string> &sequences, double t, const vector<bool> &alreadySatisfied);

// Función que analiza un conjunto de strings para encontrar la posición con mayor diversidad de caracteres
int findBestStartPosition(const vector<string> &sequences, int stringLength);

// Función que elige el siguiente carácter basado en el algoritmo greedy aleatorizado
char chooseNextCharacter(const string &currentSolution, int position, const vector<char> &alphabet, const vector<string> &sequences, double epsilon, double t, const vector<bool> &alreadySatisfied);

// Función principal que construye la solución utilizando el algoritmo greedy aleatorizado
pair<int, string> constructGreedySolution(int stringLength, const vector<char> &alphabet, const vector<string> &sequences, double epsilon, double t);

#endif // FUNCTIONS_H