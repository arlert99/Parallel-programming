# Курс «Параллельное программирование»

## Лабораторные работы

| № | Тема | Технология | Ссылка на отчёт |
| :---: | :--- | :---: | :--- |
| 1 | Последовательное перемножение матриц | — | [Отчёт](lab1/README.md) |
| 2 | Параллельное перемножение | OpenMP | [Отчёт](lab2/README.md) |
| 3 | Параллельное перемножение | MPI | [Отчет](lab3/README.md) |
| 4 | Параллельное перемножение | CUDA | *в разработке* |

## Структура репозитория

```
.
├── lab1/ # Последовательная версия
│ ├── main.cpp 
│ ├── generate.py 
│ ├── verify.py 
│ ├── graph.png 
│ └── README.md 
│
├── lab2/ # Параллельная версия (OpenMP)
│ ├── main.cpp 
│ ├── generate.py 
│ ├── verify.py 
│ ├── graph_time.png 
│ ├── graph_speedup.png 
│ └── README.md
│
├── lab3/ # MPI 
│ ├── main.cpp                 
│ ├── verify.py                 
│ ├── run_experiments.sh        
│ ├── results_mpi.csv           
│ ├── graph_time_mpi.png       
│ ├── graph_speedup_mpi.png     
│ └── README.md  
├── lab4/ # CUDA (в разработке)
│
│── .gitignore
└── README.md 
```