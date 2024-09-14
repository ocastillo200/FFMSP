# Far from most string problem (FFMSP)

Implementación de algoritmo greedy aleatorizado para la construcción de soluciones iniciales para el problema.

## Integrantes:
- Oscar Castillo
- Francisco Cea
- Macarena Madrid

## Requerimientos:
- C++11
- CMake 3.10
- Make

## Instrucciones de uso:
1. Clonar repositorio
```bash
$ git clone https://github.com/ocastillo200/FFMSP.git
```
2. Compilar
```bash
$ cd FFMSP
$ mkdir build
$ cd build
$ cmake ..
$ make
```
3. Ejecutar el programa desde la carpeta build
```bash
./greedy -<Greedy|GreedyA> -i <instancia.txt> -th <threshold {0,1}> -e <valor epsilon para desiciones aleatorias {0,1}>
```
