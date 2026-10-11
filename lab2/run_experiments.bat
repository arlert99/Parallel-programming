@echo off
echo size,threads,time > results_omp.csv

for %%S in (200 400 800 1200 1600 2000) do (
    for %%T in (1 2 4 8) do (
        echo Running: Size=%%S, Threads=%%T
        lab2.exe matrix_A_%%S.txt matrix_B_%%S.txt temp.txt %%T | findstr "RESULT_CSV" >> results_omp.csv
    )
)
echo Done