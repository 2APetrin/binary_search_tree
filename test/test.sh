#!/bin/bash

project_dir=$(dirname "$(realpath $0)")/..

obj=${project_dir}/build/src/compare

tests_folder=${project_dir}/test/tests/

test_folder=${project_dir}/test

test_files_names=( "00.dat" "01.dat" "02.dat" "03.dat" "04.dat" "05.dat" "06.dat" "07.dat" "08.dat" "09.dat" "10.dat" "11.dat" "12.dat" "13.dat" "14.dat" "15.dat")


rm ${test_folder}/tmp.txt

for i in ${!test_files_names[@]}; do
    echo
    ${obj} < ${tests_folder}${test_files_names[$i]}
done

python3 ${test_folder}/graph.py

rm ${test_folder}/tmp.txt
