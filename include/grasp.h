#ifndef GRASP_H
#define GRASP_H

#include <iostream>
#include <vector>
#include <random>
#include <functional>
#include <chrono>

/**
 * Realiza una búsqueda local en la solución inicial.
 *
 * @param initialSolution La solución inicial.
 * @param initialFitness El fitness inicial.
 * @param inputStrings Las cadenas de entrada.
 * @param alphabet El alfabeto.
 * @param t El valor de t.
 * @return std::pair<std::string, int> La mejor solución y su fitness.
 */
std::pair<std::string, int> localSearch(const std::string &initialSolution, int initialFitness, const std::vector<std::string> &inputStrings, const std::vector<char> &alphabet, double t);

/**
 * Realiza una búsqueda GRASP.
 *
 * @param inputStrings Las cadenas de entrada.
 * @param alphabet El alfabeto.
 * @param maxIterations El número máximo de iteraciones.
 * @param epsilon El valor de epsilon.
 * @param t El valor de t.
 * @param timelimit El límite de tiempo.
 * @param showLog Indica si se debe mostrar el registro.
 * @return std::pair<int, std::string> El mejor fitness y su solución.
 */
std::pair<int, std::string> GRASP(const std::vector<std::string> &inputStrings, const std::vector<char> &alphabet, int maxIterations, double epsilon, double t, double timelimit, bool showLog);

#endif