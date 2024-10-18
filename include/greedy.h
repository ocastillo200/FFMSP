#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

/**
 * Calcula la distancia de Hamming entre dos cadenas de caracteres a partir de una posición dada. La
 * distancia de Hamming es el número de posiciones en las que los caracteres correspondientes son diferentes.
 *
 * @param s1 La primera cadena de caracteres.
 * @param s2 La segunda cadena de caracteres.
 * @param position La posición inicial desde la cual se comienza a calcular la distancia de Hamming.
 * @return int La distancia de Hamming entre las dos cadenas a partir de la posición dada.
 */
int hammingDistance(const std::string &s1, const std::string &s2, int position);

/**
 * Calcula el costo de la solución actual.
 *
 * Evalúa cuántas cadenas en omega no cumplen con el umbral de similitud
 * establecido por el parámetro t.
 *
 * @param currentSolution La solución parcial actual.
 * @param omega Vector de cadenas de entrada.
 * @param t Umbral de similitud (entre 0 y 1).
 * @param position Posición desde la cual se comienza a calcular la distancia de Hamming (no se usa en esta implementación).
 * @param lenght Longitud de la solución (no se usa en esta implementación).
 * @return El número de cadenas que no cumplen con el umbral de similitud.
 */
int calculateCost(const std::string &currentSolution, const std::vector<std::string> &omega,
                  double t, int position, int lenght);

/**
 * Encuentra la mejor posición de inicio para la solución greedy.
 *
 * Se analiza el conjunto de cadenas de entrada (omega) para determinar
 * la posición que tiene la menor diversidad de caracteres. Esta posición se considera
 * la mejor para iniciar la construcción de la solución greedy.
 *
 * @param omega Vector de cadenas de entrada.
 * @param stringLength Longitud de cada cadena en omega.
 * @return int La mejor posición de inicio (índice) para la solución greedy.
 */
int findBestStartPosition(const std::vector<std::string> &sequences, int stringLength);

/**
 * Elige el siguiente carácter para la solución greedy.
 *
 * Cuando epsilon es distinto de 1, el método alterna entre la selección greedy y la selección
 * aleatoria, lo que constituye el enfoque greedy aleatorizado. Esto permite explorar un espacio
 * de soluciones más amplio y potencialmente escapar de óptimos locales.
 *
 * @param currentSolution La solución parcial actual en construcción.
 * @param position La posición actual en la que se está eligiendo el carácter.
 * @param alphabet El alfabeto de caracteres disponibles.
 * @param omega El conjunto de cadenas de entrada.
 * @param epsilon El factor de aleatorización (entre 0 y 1).
 * @param t El umbral de similitud para considerar una cadena como satisfecha.
 * @param lenght La longitud actual de la solución parcial.
 * @param bestStartPosition La mejor posición de inicio determinada previamente.
 * @return El carácter elegido para la posición actual.
 */
char chooseNextCharacter(const std::string &currentSolution, int position,
                         const std::vector<char> &alphabet,
                         const std::vector<std::string> &omega, double epsilon, double t,
                         int lenght,
                         int bestStartPosition);

/**
 *
 * Se mplementa un algoritmo greedy para construir una solución al problema FFMSP
 * El algoritmo comienza desde la mejor posición de inicio y construye la solución
 * carácter por carácter, eligiendo el siguiente carácter de forma greedy o aleatoria
 * según el valor de epsilon.
 *
 * @param stringLength Longitud de la cadena solución.
 * @param alphabet Vector de caracteres que representan el alfabeto disponible.
 * @param omega Vector de cadenas de entrada.
 * @param epsilon Factor de aleatorización para el algoritmo greedy.
 * @param t Umbral de similitud para considerar una cadena como satisfecha.
 * @return pair<int, string> Par que contiene la calidad de la solución y la cadena solución.
 */
std::pair<int, std::string> constructGreedySolution(int stringLength,
                                                    const std::vector<char> &alphabet,
                                                    const std::vector<std::string> &omega,
                                                    double epsilon, double t);

#endif