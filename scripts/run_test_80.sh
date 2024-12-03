#! /bin/bash

# Metricas obtenidas con irace

th_list=(0.80)
cases=(100 200)

test_path="../build/tests"
instances_path="../instancias/"

for case in "${cases[@]}"; do
    for mode in "hybrid"; do
        mkdir -p results
        output_file="results/${mode}-${case}-0.80.txt"
        rm -f $output_file    # 
        for th in "${th_list[@]}"; do
            if [ "$th" != "${th_list[0]}" ]; then
                echo "\n" >> "$output_file"
            fi
            $test_path -th $th -n $case -mode $mode -f $instances_path -t 10 -e 0.99 -p 300 -m 0.17 -c 0.19 -g 1000 -tn 1 >> "$output_file"
        done
    done
done