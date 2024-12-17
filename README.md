# Far from most string problem (FFMSP)

Implementación de algoritmo evolutivo para la construcción de soluciones mejoradas para el problema a partir de poblaciones generadas con greedy aleatorizado.

## Integrantes:
- Oscar Castillo
- Francisco Cea
- Macarena Madrid
- Viktoriya Koleva

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
./hybrid -i ../<instancia.txt> -th <threshold {0,1}> -e <valor epsilon para desiciones aleatorias {0,1}> -t <tiempo límite> -p <población> -g <generaciones>  -m <mutation rate {0,1}> -c <crossover rate {0,1}>
```
