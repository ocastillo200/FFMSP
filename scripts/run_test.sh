#! /bin/bash

# Este script ejecuta los tests del algoritmo para diferentes configuraciones.
# Realiza pruebas con distintos umbrales, tamaños de casos y modos (Greedy y GreedyA),
# guardando los resultados en archivos separados para su posterior análisis.

th_list=(0.75 0.8 0.85)
cases=(100 200)
epsilon=0.85

test_path="../build/tests"
instances_path="../instancias/"

for case in "${cases[@]}"; do
    for mode in "greedy" "greedyA" "grasp"; do
        mkdir -p results
        output_file="results/${mode}-${case}-${epsilon}.txt"
        rm -f $output_file    # 
        for th in "${th_list[@]}"; do
            if [ "$th" != "${th_list[0]}" ]; then
                echo "\n" >> "$output_file"
            fi
            if [ "$mode" = "greedy" ]; then
                $test_path -th $th -n $case -mode $mode -f $instances_path >> "$output_file"
            else 
                $test_path -th $th -n $case -mode $mode -e $epsilon -f $instances_path -t 10 >> "$output_file"
            fi
        done
    done
done