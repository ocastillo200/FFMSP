#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

/**
 * Esta función busca un parámetro específico en la lista de argumentos de la línea de comandos
 * y devuelve su valor asociado.
 *
 * @param argc El número de argumentos de la línea de comandos.
 * @param argv Un vector que contiene los argumentos de la línea de comandos.
 * @param key La clave del parámetro que se desea buscar.
 * @return El valor del parámetro asociado a la clave proporcionada. Si la clave no se encuentra,
 *         se devuelve una cadena vacía.
 */
std::string readParam(int argc, const std::vector<std::string> &argv, const std::string &key);


/**
 * Verifica si un argumento en específico se encuentra presente en los argumentos
 * proporcionados.
 *
 * @param argc Número de argumentos.
 * @param argv Vector de cadenas que contiene los argumentos.
 * @param key La bandera que se desea buscar.
 * @return true Si la bandera está presente en los argumentos.
 * @return false Si la bandera no está presente en los argumentos.
 */
bool containsFlag(int argc, const std::vector<std::string> &argv, const std::string &key);


/**
 * Obtiene la longitud de una cadena a partir del nombre de un archivo. La longitud
 * se encuentra entre dos guiones en el nombre del archivo.
 *
 * @param filename El nombre del archivo como una cadena de caracteres.
 * @return int La longitud de la cadena del nombre del archivo.
 */
int getStringLengthFromFilename(const std::string &filename);

#endif