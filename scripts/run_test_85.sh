#! /bin/bash

# Metricas obtenidas con irace

th_list=(0.85)
cases=(100 200)

test_path="../build/tests"
instances_path="../instancias/"

for case in "${cases[@]}"; do
    for mode in "hybrid"; do
        mkdir -p results
        output_file="results/${mode}-${case}-0.85.txt"
        rm -f $output_file    # 
        for th in "${th_list[@]}"; do
            if [ "$th" != "${th_list[0]}" ]; then
                echo "\n" >> "$output_file"
            fi
            $test_path -th $th -n $case -mode $mode -f $instances_path -t 60 -e 0.74 -p 1000 -m 0.02 -c 0.38 -g 1000 -tn 1 >> "$output_file"
        done
    done
done